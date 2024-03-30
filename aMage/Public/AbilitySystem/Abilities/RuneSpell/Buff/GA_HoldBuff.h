// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MainGenericGameplayAbility.h"
#include "GA_HoldBuff.generated.h"

/**
 * 
 */
UCLASS()
class AMAGE_API UGA_HoldBuff : public UMainGenericGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	virtual void ActivateAbilityAfterHeld() override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable)
	virtual void RemoveActiveEffects();
	
	UFUNCTION(Server,Reliable)
	void ServerRemoveActiveEffect();
	
protected:
	
	bool bIsInputHeld=false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Effect,meta=(AllowPrivateAccess=true))
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Effect,meta=(AllowPrivateAccess=true))
	FActiveGameplayEffectHandle ActiveHandle;
	
private:

	
};
