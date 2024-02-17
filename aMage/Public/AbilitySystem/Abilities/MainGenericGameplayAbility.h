// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MainGameplayAbility.h"
#include "MainGenericGameplayAbility.generated.h"

class UMainInputAction;
/**
 * 
 */
UCLASS()
class AMAGE_API UMainGenericGameplayAbility : public UMainGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
protected:
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category=Spell)
	float UsageTimes;
	
	UPROPERTY(EditAnywhere,Category=MatchRuneTag)
	TObjectPtr<UMainInputAction> LMBInputAction;
	UPROPERTY(EditAnywhere,Category=MatchRuneTag)
	TObjectPtr<UMainInputAction> RMBInputAction;
	
	UPROPERTY(EditDefaultsOnly,Category="Effects")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToRemoveOnEnd;
	
	UPROPERTY(EditDefaultsOnly,Category="Effects")
	TArray<TSubclassOf<UGameplayEffect>>OngoingEffectsToApplyOnstart;

	TArray<FActiveGameplayEffectHandle> RemoveOnEndEffectHandle;

};
