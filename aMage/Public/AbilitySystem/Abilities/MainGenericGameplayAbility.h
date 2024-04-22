// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/MainDamageGameplayAbility.h"
#include "MainGenericGameplayAbility.generated.h"

class UInterpolateFOV;
class AMainPlayerCharacter;
class UMainPlayerWidget;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUsageTimeChangedSignature,int,UsageTimes);
UCLASS()
class AMAGE_API UMainGenericGameplayAbility : public UMainDamageGameplayAbility
{
	GENERATED_BODY()

public:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	
	virtual void PostInitProperties();
	
	UPROPERTY(BlueprintAssignable,Category=Spell)
	FOnUsageTimeChangedSignature OnUsageTimeChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Spell")
	void TriggerUsageTimeChanged(float NewUsageTime)
	{
		OnUsageTimeChanged.Broadcast(NewUsageTime);
	}
	
	UFUNCTION(Client,Reliable)
	void ClientUpdateTriggerUsageTimeChanged(float NewUsageTime);
	
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
		
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	//Camera
	virtual void ToggleCameraFOV(bool IsActivate);
	UFUNCTION()
	void ManualEndAbility();
	
	//Ability
	UFUNCTION(BlueprintCallable)
	void RemoveAbilityAfterEnd(const TArray<TSubclassOf<UGameplayAbility>>& RemoveAbilities);

	//Cue
	void K2_RemoveGameplayCue(FGameplayTag GameplayCueTag) override;
	
protected:
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Player)
	TObjectPtr<AMainPlayerCharacter> MainPlayerCharacter;
	
	//Ability 
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadOnly,Category=Spell)
	bool bIsCancel=false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Spell)
	float MaxUsageTimes = 0;
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category=Spell)
	float UsageTimes=0;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Spell)
	float RefillPercentage = 0.33f;

	//Cue When First Get Ability
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(GameplayTagFilter="GameplayCue"),Category=Cue)
	FGameplayTag ActivationGameplayCueTag;
	
	//Camera
	UPROPERTY(EditDefaultsOnly,Category = Camera,meta=(AllowPrivateAccess=true))
	float DefaultFOV= 90.f;
	UPROPERTY(EditDefaultsOnly,Category = Camera,meta=(AllowPrivateAccess=true))
	float NewFOV= 90.f;
	UPROPERTY(EditDefaultsOnly,Category = Camera,meta=(AllowPrivateAccess=true))
	float Duration = 0.5f;
	TObjectPtr<UInterpolateFOV> InterpFOVTask;
	
	//UI
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=UI)
	TSubclassOf<UMainPlayerWidget> SpellIndicatorClass;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=UI)
	TObjectPtr<UMainPlayerWidget> SpellIndicator;

	// Input
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadOnly,Category=Timer)
	bool bIsHeldLoop=false;
	UFUNCTION()
	virtual void ActivateAbilityAfterHeld();
	UPROPERTY()
	FTimerHandle TimerHandle_InputHeld;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Timer)
	float InputHeldDuration = 3.0f; // Hold duration in seconds
	
public:
	void RefillUsageTime();
};
