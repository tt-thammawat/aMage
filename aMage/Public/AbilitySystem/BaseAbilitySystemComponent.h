// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags,const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FOnAbilityGranted);

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
	//Ability was granted
	FOnAbilityGranted OnAbilityGranted;
	//Add Ability For Character
	UFUNCTION(BlueprintCallable,Category="Abilities")
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AddAbilities,bool bIsAddedByDrawing=false);
	UFUNCTION(BlueprintCallable,Category="Abilities")
	void RemoveCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& RemoveAbilities);
	//Use This When Remove Ability via item etc. that not directly from ability itself
	UFUNCTION(BlueprintCallable,Category="Abilities")
	void RemoveNormalRuneSpellAbilities();
	//Input Tags For Abilities
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
protected:
	//Apply Effect For Delegate Function
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);


};
