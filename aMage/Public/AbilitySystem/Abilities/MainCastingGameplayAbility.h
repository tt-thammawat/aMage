// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MainGameplayAbility.h"
#include "MainCastingGameplayAbility.generated.h"

class UInterpolateFOV;
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

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

private:
	//Speed
	UPROPERTY(EditDefaultsOnly,Category = Default,meta=(AllowPrivateAccess=true))
	float SlowMaxWalkSpeed = 350.f;
	float OldMaxWalkSpeed = 0.f;
	//Camera
	UPROPERTY(EditDefaultsOnly,Category = Default,meta=(AllowPrivateAccess=true))
	float DefaultFOV= 90.f;
	UPROPERTY(EditDefaultsOnly,Category = Default,meta=(AllowPrivateAccess=true))
	float NewFOV= 90.f;
	UPROPERTY(EditDefaultsOnly,Category = Default,meta=(AllowPrivateAccess=true))
	float Duration = 0.5f;
	TObjectPtr<UInterpolateFOV> InterpFOVTask;
	
	UPROPERTY(EditDefaultsOnly,Category = Default,meta=(AllowPrivateAccess=true))
	float DefaultVignetteIntensity= 0.4f;
	UPROPERTY(EditDefaultsOnly,Category = Default,meta=(AllowPrivateAccess=true))
	float NewVignetteIntensity= 1.5f;
	
	UPROPERTY(EditDefaultsOnly,Category = Default,meta=(AllowPrivateAccess=true))
	float DefaultDepthOfFieldVignetteSize= 200.f;
	UPROPERTY(EditDefaultsOnly,Category = Default,meta=(AllowPrivateAccess=true))
	float NewDepthOfFieldVignetteSize= 200.f;
	
	//RuneTag
	UFUNCTION()
	void AddRuneTags();
	UFUNCTION()
	void ClearRuneTags();
	TObjectPtr<UMainPaintWidget> PaintWidget;
	// Tracks whether the ability is currently active.
	bool bIsAbilityActive = false;
	// Prevent double press
	bool bIsDebouncing = false;
	void ToggleDrawingMode(bool IsActivate);
	UFUNCTION()
	void ManualEndAbility();
	void ActivateDrawingMode();
	void DeactivateDrawingMode();
};
