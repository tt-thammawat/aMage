// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "GameplayTagsSingleton.h"
#include "AbilitySystem/Abilities/MainGameplayAbility.h"


void UBaseAbilitySystemComponent::AbilityActorInfoSet()
{
	//Add Delegate When Affect Applies to self
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UBaseAbilitySystemComponent::ClientEffectApplied);
}

void UBaseAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AddAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : AddAbilities)
	{
		
		// init FGameplayAbilitySpec Struct
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		//if Ability is derived from MainGameplayAbility it will have InputTag Button 1 23 4 LMB RMB
		if(const UMainGameplayAbility* MainGameplayAbility = Cast<UMainGameplayAbility>(AbilitySpec.Ability))
		{
			MainGameplayAbility->StartupInputTag;
			//Add InputTag from StartupInputTag from MainGameplayAbility To DynamicAbilityTags ( FGameplayContainer )
			AbilitySpec.DynamicAbilityTags.AddTag(MainGameplayAbility->StartupInputTag);
			//Add StartUp Abilities if it derives from MainGameplayAbility
			GiveAbility(AbilitySpec);
			//	GiveAbilityAndActivateOnce(AbilitySpec);
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
			// ItemAbility->StartupInputTag;
			// AbilitySpec.DynamicAbilityTags.RemoveTag(ItemAbility->StartupInputTag);
			ClearAbility(AbilitySpec->Handle);
		}
	}
}


void UBaseAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;
		// Check If We Have Activatable Abilities
		for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
		{
			if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				//If a match is found, it first marks the ability as pressed [GAS]
				AbilitySpecInputPressed(AbilitySpec);
				if(!AbilitySpec.IsActive())
				{
					//TryActivate Abilities may fail if logic failed
					TryActivateAbility(AbilitySpec.Handle);
				}
			}
		}
}

void UBaseAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//If a match is found, it marks the ability as released [GAS]
			AbilitySpecInputReleased(AbilitySpec);
		}
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
