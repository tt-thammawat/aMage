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

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

private:
	//Speed
	UPROPERTY(EditDefaultsOnly,Category = Default,meta=(AllowPrivateAccess=true))
	float OldMaxWalkSpeed = 600.f;
	UPROPERTY(EditDefaultsOnly,Category = Default,meta=(AllowPrivateAccess=true))
	float MaxWalkSpeedMultiplier = 0.8f;
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
	void CheckRuneTags();
	UFUNCTION()
	void ClearRuneTags();
	UFUNCTION()
	void ReloadRuneTags();
	//Check if the ability is active if no then reset visual
	void CheckNormalSpellAbilityTag();
	// Tracks whether the ability is currently active by Input.
	bool bIsAbilityActive = false;
	// Prevent double press
	bool bIsDebouncing = false;
	//Check Hold
	FTimerHandle HoldDetectionTimerHandle;
	bool bIsHoldDetected = false;
	// Held Mode
	bool bIsHeldModeActive = false;
	void ToggleDrawingMode(bool IsActivate);
	UFUNCTION(Client, Reliable)
	void ClientCancelAbilities();
	
	UFUNCTION()
	void ManualEndAbility();
	void ActivateDrawingMode();
	UFUNCTION()
	void DeactivateDrawingMode();

public:
	//Paint Widget
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Spell)
	TSubclassOf<UUserWidget> PaintWidgetClass;
	TObjectPtr<UMainPaintWidget> PaintWidget;
};
