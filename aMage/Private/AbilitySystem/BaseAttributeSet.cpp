// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "GameplayTagsSingleton.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

UBaseAttributeSet::UBaseAttributeSet()
{
	//Get Tag Singleton
	const FMainGameplayTags& MainGameplayTags = FMainGameplayTags::Get();
	TagsToAttributes.Add(MainGameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(MainGameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(MainGameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(MainGameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	TagsToAttributes.Add(MainGameplayTags.Attributes_Secondary_FireRes, GetFireResAttribute);
	TagsToAttributes.Add(MainGameplayTags.Attributes_Secondary_IceRes, GetIceResAttribute);
	TagsToAttributes.Add(MainGameplayTags.Attributes_Secondary_ElectricRes, GetElectricResAttribute);
	TagsToAttributes.Add(MainGameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(MainGameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);

	TagsToAttributes.Add(MainGameplayTags.Attributes_Vital_Health, GetHealthAttribute);
	TagsToAttributes.Add(MainGameplayTags.Attributes_Vital_Mana, GetManaAttribute);


}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		//Primary Attributes
		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,Strength,COND_None,REPNOTIFY_Always);
		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,Intelligence,COND_None,REPNOTIFY_Always);
		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,Resilience,COND_None,REPNOTIFY_Always);
		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,Vigor,COND_None,REPNOTIFY_Always);
		//Secondary Attribures
		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,FireRes,COND_None,REPNOTIFY_Always);
		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,IceRes,COND_None,REPNOTIFY_Always);
		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,ElectricRes,COND_None,REPNOTIFY_Always);
		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);

		// Replicate Health Without Condition Update with REPNOTIFY Always
		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,Health,COND_None,REPNOTIFY_Always);
		//MANA
		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,Mana,COND_None,REPNOTIFY_Always);
		

	}
}

//Set Effect With Source And Target Information
void UBaseAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	//Source = Causer of the effect, target = target of the effect Owner of this AS

	Props.EffectContextHandle =Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	
	if(IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if(	Props.SourceController ==nullptr && Props.SourceAvatarActor != nullptr)
		{
			if(const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if(Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}


void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	//Clamp Value when got hit
	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxHealth());
	}
	
	if(Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxMana());
	}
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Now You Have The instigator and target Information
	FEffectProperties Props;
	SetEffectProperties(Data,Props);
	//Champ Here Again TO Not Change The Value Above The scope
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(),0.f,GetMaxHealth()));
		FString Message = FString::Printf(TEXT("Changed Health On %s, Health %f"), *Props.TargetAvatarActor->GetName(), GetHealth());
		UKismetSystemLibrary::PrintString(GetWorld(), Message, true, false, FLinearColor::Red, 3.0f);
	}
	if(Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(),0.f,GetMaxMana()));
	}}

void UBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	// Register Change To All Client
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health, OldHealth);
}

void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldMaxHealth);
}

void UBaseAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet,Mana,OldMana);

}

void UBaseAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet,MaxMana,OldMaxMana);
}

void UBaseAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet,Strength,OldStrength);
}

void UBaseAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet,Intelligence,OldIntelligence);
}

void UBaseAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet,Resilience,OldResilience);
}

void UBaseAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet,Vigor,OldVigor);
}

void UBaseAttributeSet::OnRep_FireRes(const FGameplayAttributeData& OldFireRes) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet,FireRes,OldFireRes);
}

void UBaseAttributeSet::OnRep_IceRes(const FGameplayAttributeData& OldIceRes) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet,IceRes,OldIceRes);
}

void UBaseAttributeSet::OnRep_ElectricRes(const FGameplayAttributeData& OldElectricRes) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet,ElectricRes,OldElectricRes);

}



