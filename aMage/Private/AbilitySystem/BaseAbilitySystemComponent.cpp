// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "GameplayTagsSingleton.h"
#include "AbilitySystem/Abilities/MainGameplayAbility.h"
#include "AbilitySystem/Abilities/MainGenericGameplayAbility.h"


void UBaseAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UBaseAbilitySystemComponent::ClientEffectApplied);
}

void UBaseAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AddAbilities,UObject* SourceObject,bool bHaveAbilityWidget)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : AddAbilities)
	{
		FGameplayAbilitySpec* ExistingSpec = FindAbilitySpecFromClass(AbilityClass);
		if(ExistingSpec == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1,INDEX_NONE,SourceObject);
			
			if(const UMainGameplayAbility* MainGameplayAbility = Cast<UMainGameplayAbility>(AbilitySpec.Ability))
			{
				AbilitySpec.DynamicAbilityTags.AddTag(MainGameplayAbility->StartupInputTag);
			}
			FGameplayAbilitySpecHandle Handle = GiveAbility(AbilitySpec);
				
			if(bHaveAbilityWidget)
			{
				if(Handle.IsValid())
				{
					TryActivateAbility(Handle);
				}
			}
			OnAbilityGranted.Broadcast();
		}
	}
}

void UBaseAbilitySystemComponent::RemoveCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& RemoveAbilities)
{
		for (const TSubclassOf<UGameplayAbility> AbilityClass : RemoveAbilities)
		{
			FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AbilityClass);
			if(AbilitySpec)
			{
				if (AbilitySpec->IsActive())
				{
					CancelAbilitySpec(*AbilitySpec,nullptr);
				}
				else
				{
					ClearAbility(AbilitySpec->Handle);
				}
			}
		}
}

void UBaseAbilitySystemComponent::RemoveNormalRuneSpellAbilities()
{
	TArray<FGameplayAbilitySpec> ActiveAbilities = GetActivatableAbilities();
	for (FGameplayAbilitySpec& AbilitySpec : ActiveAbilities)
	{
		if (AbilitySpec.Ability->AbilityTags.HasTagExact(FMainGameplayTags::Get().Ability_Rune_NormalSpell))
		{
				if (AbilitySpec.IsActive())
				{
					CancelAbilitySpec(AbilitySpec,nullptr);
				}
				else
				{
					ClearAbility(AbilitySpec.Handle);
				}
		}
	}
}

void UBaseAbilitySystemComponent::RemovePotionAbilities()
{
	TArray<FGameplayAbilitySpec> ActiveAbilities = GetActivatableAbilities();
	for (FGameplayAbilitySpec& AbilitySpec : ActiveAbilities)
	{
		if (AbilitySpec.Ability->AbilityTags.HasTagExact(FMainGameplayTags::Get().Ability_Buff_Potion))
		{
			ClearAbility(AbilitySpec.Handle);
		}
	}
}

void UBaseAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	TArray<FGameplayAbilitySpecHandle> MatchingAbilities;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			MatchingAbilities.Add(AbilitySpec.Handle);
		}
	}

	for (const FGameplayAbilitySpecHandle& AbilityHandle : MatchingAbilities)
	{
		FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilityHandle);
		if (AbilitySpec)
		{
			AbilitySpecInputPressed(*AbilitySpec);
			if (!AbilitySpec->IsActive())
			{
				TryActivateAbility(AbilityHandle);
			}
		}
	}
}

void UBaseAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	TArray<FGameplayAbilitySpec*> MatchingAbilitySpecs;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.IsActive() && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			MatchingAbilitySpecs.Add(&AbilitySpec);
		}
	}

	for (FGameplayAbilitySpec* AbilitySpec : MatchingAbilitySpecs)
	{
		AbilitySpecInputReleased(*AbilitySpec);
	}
}

void UBaseAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	{
		FGameplayTagContainer TagContainer;
		EffectSpec.GetAllAssetTags(TagContainer);

		EffectAssetTags.Broadcast(TagContainer);
	}
}
