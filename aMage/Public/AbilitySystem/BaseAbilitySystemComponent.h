// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags,const FGameplayTagContainer& /*AssetTags*/);
/**
 * 
 */
UCLASS()
class AMAGE_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	//Set ASC For Actor
	void AbilityActorInfoSet();

	//Set Effect Tag
	FEffectAssetTags EffectAssetTags;

	//Add Ability From Character
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities);
	//Input Tags For Abilities
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
protected:
	//Apply Effect For Delegate Function
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
