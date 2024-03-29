// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "GameplayTagsSingleton.h"
#include "AbilitySystem/MainAbilitySystemLibrary.h"
#include "Character/MainPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interact/ICombatInterface.h"
#include "Net/UnrealNetwork.h"

UBaseAttributeSet::UBaseAttributeSet()
{
	//Get Tag Singleton
	const FMainGameplayTags& MainGameplayTags = FMainGameplayTags::Get();
	TagsToAttributes.Add(MainGameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(MainGameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(MainGameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(MainGameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);
	TagsToAttributes.Add(MainGameplayTags.Attributes_Secondary_MaxMoveSpeed, GetMaxMoveSpeedAttribute);

	TagsToAttributes.Add(MainGameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(MainGameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);

	TagsToAttributes.Add(MainGameplayTags.Attributes_Resistance_Fire, GetResistanceFireAttribute);
	TagsToAttributes.Add(MainGameplayTags.Attributes_Resistance_Lightning, GetResistanceLightningAttribute);
	TagsToAttributes.Add(MainGameplayTags.Attributes_Resistance_Ice, GetResistanceIceAttribute);
	TagsToAttributes.Add(MainGameplayTags.Attributes_Resistance_Physical, GetResistancePhysicalAttribute);
	
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
		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,MaxMoveSpeed,COND_None,REPNOTIFY_Always);

		//Secondary Attributes
		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);

		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,ResistanceFire,COND_None,REPNOTIFY_Always);
		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,ResistanceLightning,COND_None,REPNOTIFY_Always);
		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,ResistanceIce,COND_None,REPNOTIFY_Always);
		DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet,ResistancePhysical,COND_None,REPNOTIFY_Always);
		
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

	//For Durations Effect That Change Movement Speed for the durations
	if (Attribute == GetMaxMoveSpeedAttribute())
	{
		ACharacter* OwningCharacter = Cast<ACharacter>(GetActorInfo()->AvatarActor);
		UCharacterMovementComponent* CharacterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;

		if(CharacterMovement)
		{
			CharacterMovement->MaxWalkSpeed = NewValue;
		}
	}
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Now You Have The instigator and target Information
	FEffectProperties Props;
	//Set Who is the Causer[Source] And Who is the Target[This]
	SetEffectProperties(Data,Props);
	//Clamp Here Again TO Not Change The Value Above The scope
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(),0.f,GetMaxHealth()));
	}
	
	if(Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(),0.f,GetMaxMana()));
	}
	
	if(Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		UAbilitySystemComponent* SourceASC = Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();
		AActor* InstigatorActor = nullptr;

		if (SourceASC)
		{
			InstigatorActor = SourceASC->GetAvatarActor();
		}
		
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if(LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth()- LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth,0.f,GetMaxHealth()));
			const bool bFatal = NewHealth <= 0.f;
			if(bFatal)
			{
				IICombatInterface* CombatInterface = Cast<IICombatInterface>(Props.TargetAvatarActor);
				if(CombatInterface && !IICombatInterface::Execute_IsDead(Props.TargetAvatarActor))
				{
					CombatInterface->Die(InstigatorActor);
				}
			}
			else
			{
				//Activate Abilities By Tags an ability that have HitReact Tag
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FMainGameplayTags::Get().Effects_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

			const bool bIsFire = UMainAbilitySystemLibrary::IsFireDamage(Props.EffectContextHandle);
			const bool bIsLightning = UMainAbilitySystemLibrary::IsLightningDamage(Props.EffectContextHandle);
			const bool bIsIce = UMainAbilitySystemLibrary::IsIceDamage(Props.EffectContextHandle);
			const bool bIsPhysical = UMainAbilitySystemLibrary::IsPhysicalDamage(Props.EffectContextHandle);
			
			ShowFloatingText(Props,LocalIncomingDamage,bIsFire,bIsLightning,bIsIce,bIsPhysical);
		}
	}
}
 
void UBaseAttributeSet::ShowFloatingText(const FEffectProperties& Props,float Damage,bool bIsFireDamage,bool bIsLightningDamage,bool bIsIceDamage,bool bIsPhysicDamage) const
{
	//Check if Damage To Self
	if(Props.SourceCharacter != Props.TargetCharacter)
	{
		//PC Hit Other
		if(AMainPlayerController* PC=Cast<AMainPlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage,Props.TargetCharacter,bIsFireDamage,bIsLightningDamage,bIsIceDamage,bIsPhysicDamage);
			return;
		}
		//PC Got Hit
		if(AMainPlayerController* PC=Cast<AMainPlayerController>(Props.TargetCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage,Props.TargetCharacter,bIsFireDamage,bIsLightningDamage,bIsIceDamage,bIsPhysicDamage);
		}
	}
}

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

void UBaseAttributeSet::OnRep_MaxMoveSpeed(const FGameplayAttributeData& OldMaxMoveSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet,MaxMoveSpeed,OldMaxMoveSpeed);

	ACharacter* OwningCharacter = Cast<ACharacter>(GetActorInfo()->AvatarActor);
	UCharacterMovementComponent* CharacterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;
	
	if(CharacterMovement)
	{
		const float MaxSpeed = GetMaxMoveSpeed();
		CharacterMovement->MaxWalkSpeed = MaxSpeed;
	}
}

void UBaseAttributeSet::OnRep_ResistanceFire(const FGameplayAttributeData& OldResistanceFire) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet,ResistanceFire,OldResistanceFire);

}

void UBaseAttributeSet::OnRep_ResistanceLightning(const FGameplayAttributeData& OldResistanceLightning) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet,ResistanceLightning,OldResistanceLightning);

}

void UBaseAttributeSet::OnRep_ResistanceIce(const FGameplayAttributeData& OldResistanceIce) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet,ResistanceIce,OldResistanceIce);

}

void UBaseAttributeSet::OnRep_ResistancePhysical(const FGameplayAttributeData& OldResistancePhysical) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet,ResistancePhysical,OldResistancePhysical);

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



