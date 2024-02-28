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
	if (!InputTag.IsValid()) return;

	// Temporary array to store ability spec handles that match the input tag
	TArray<FGameplayAbilitySpecHandle> MatchingAbilities;

	// First pass: Identify abilities that match the criteria
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			MatchingAbilities.Add(AbilitySpec.Handle);
		}
	}

	// Second pass: Act on the collected abilities
	for (const FGameplayAbilitySpecHandle& AbilityHandle : MatchingAbilities)
	{
		// Retrieve the spec again; it's safe because we're not iterating over the collection here
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

	// Temporary array to store matching ability specs
	TArray<FGameplayAbilitySpec*> MatchingAbilitySpecs;

	// First pass: collect abilities
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.IsActive() && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			MatchingAbilitySpecs.Add(&AbilitySpec);
		}
	}

	// Second pass: perform actions on collected abilities
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
