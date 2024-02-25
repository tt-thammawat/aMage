// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MainGameplayAbility.h"
#include "Interact/CastingInterface.h"
#include "MainCastingGameplayAbility.generated.h"

class UAbilityTask_PlayMontageAndWait;
class UMainPaintWidget;
/**
 * 
 */
UCLASS()
class AMAGE_API UMainCastingGameplayAbility : public UMainGameplayAbility
{
	GENERATED_BODY()
public:
	UMainCastingGameplayAbility();
protected:
	
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

private:
	UPROPERTY(EditDefaultsOnly,Category = Default,meta=(AllowPrivateAccess=true))
	float SlowMaxWalkSpeed = 350.f;
	float OldMaxWalkSpeed = 0.f;
	UFUNCTION()
	void AddRuneTags(FGameplayTag RuneTag);
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess="true"))
	TArray<FGameplayTag> RuneTags;
	TObjectPtr<UMainPaintWidget> PaintWidget;
};
