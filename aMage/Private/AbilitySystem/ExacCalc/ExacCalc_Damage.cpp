// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExacCalc/ExacCalc_Damage.h"

#include "GameplayTagsSingleton.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/MainAbilitySystemLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interact/ICombatInterface.h"

struct MainDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceFire);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceLightning);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceIce);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistancePhysical);

	//Map for for loop
	TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	// Only run once this struct construct
	MainDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,ResistanceFire,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,ResistanceLightning,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,ResistanceIce,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,ResistancePhysical,Target,false);

		const FMainGameplayTags& Tags = FMainGameplayTags::Get();
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire,ResistanceFireDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning,ResistanceLightningDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Ice,ResistanceIceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical,ResistancePhysicalDef);
	}
};

// Function to access the singleton instance
// Declaring and using the Singleton pattern within a .cpp file
// encapsulates its use to that file alone

static const MainDamageStatics& DamageStatics()
{
	static MainDamageStatics DStatics;
	return DStatics;
}

UExacCalc_Damage::UExacCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceFireDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceLightningDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceIceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistancePhysicalDef);
}

void UExacCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC=ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC=ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	IICombatInterface* SourceCombatInterface = Cast<IICombatInterface>(SourceAvatar);
	IICombatInterface* TargetCombatInterface = Cast<IICombatInterface>(TargetAvatar);

	//Source Spec
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters AggregatorEvaluateParameters;
	AggregatorEvaluateParameters.SourceTags = SourceTags;
	AggregatorEvaluateParameters.TargetTags = TargetTags;

	const UCharacterClassInfo* CharacterClassInfo = UMainAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

	//Get Damage Set By Caller Magnitude [Source]
	float Damage = 0.f;
	for (const  TTuple<FGameplayTag, FGameplayTag>& Pair  : FMainGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		FGameplayEffectContextHandle EffectContextHandle =  Spec.GetContext();

		//TODO: Change These To Enum
		// Reset damage types to false initially
		UMainAbilitySystemLibrary::SetIsFireDamage(EffectContextHandle, false);
		UMainAbilitySystemLibrary::SetIsLightningDamage(EffectContextHandle, false);
		UMainAbilitySystemLibrary::SetIsIceDamage(EffectContextHandle, false);
		UMainAbilitySystemLibrary::SetIsPhysicalDamage(EffectContextHandle, false);
		
		// Then set the appropriate damage type to true
		if (DamageTypeTag == FMainGameplayTags::Get().Damage_Fire)
		{
			UMainAbilitySystemLibrary::SetIsFireDamage(EffectContextHandle, true);
		}
		if (DamageTypeTag == FMainGameplayTags::Get().Damage_Lightning)
		{
			UMainAbilitySystemLibrary::SetIsLightningDamage(EffectContextHandle, true);
		}
		if (DamageTypeTag == FMainGameplayTags::Get().Damage_Ice)
		{
			UMainAbilitySystemLibrary::SetIsIceDamage(EffectContextHandle, true);
		}
		
		if (DamageTypeTag == FMainGameplayTags::Get().Damage_Physical)
		{
			UMainAbilitySystemLibrary::SetIsPhysicalDamage(EffectContextHandle, true);
		}
		
		checkf(MainDamageStatics().TagsToCaptureDefs.Contains(ResistanceTag),TEXT("TagsToCaptureDefs doesn't Contain Tag : %s in ExecCalc_Damage"),*ResistanceTag.ToString());
		//Capture Value by using KEY (FGameplayTag)
		const FGameplayEffectAttributeCaptureDefinition CaptureDefinition = MainDamageStatics().TagsToCaptureDefs[ResistanceTag];
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);
		
		float Resistance = 0.f;
		
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition,AggregatorEvaluateParameters,Resistance);
		Resistance = FMath::Clamp(Resistance,0.f,100.f);
		//resistance = 10 -> (100-10)/100 = 90
		DamageTypeValue *= (100.f - Resistance)/100.f;
		Damage += DamageTypeValue;
	}
	
	const FGameplayModifierEvaluatedData EvaluatedData(UBaseAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive,Damage);
	// send FGameplayEffectCustomExecutionOutput Out
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
