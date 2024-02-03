// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAbilitySystemComponent.h"

void UBaseAbilitySystemComponent::AbilityActorInfoSet()
{
	//Add Delegate When Affect Applies to self
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UBaseAbilitySystemComponent::ClientEffectApplied);
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