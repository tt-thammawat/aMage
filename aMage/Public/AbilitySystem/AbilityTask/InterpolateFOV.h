// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "InterpolateFOV.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInterpolationCompleted);

class UCameraComponent;
/**
 * 
 */
UCLASS()
class AMAGE_API UInterpolateFOV : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Interpolate FOV", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UInterpolateFOV* InterpolateFOV(UGameplayAbility* OwningAbility, UCameraComponent* CameraComponent, float InitialFOV, float TargetFOV, float Duration);

	
	FOnInterpolationCompleted OnInterpolationCompleted;

protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	void InterpolationTick();

	UPROPERTY()
	UCameraComponent* CameraComponent;

	float InitialFOV;
	float TargetFOV;
	float InterpolationDuration;
	float StartTime;

	FTimerHandle TimerHandle;
};
