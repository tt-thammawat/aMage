// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderLineTrace.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLineTraceDelegate,const FGameplayAbilityTargetDataHandle& ,DataHandle);
/**
 * 
 */
UCLASS()
class AMAGE_API UTargetDataUnderLineTrace : public UAbilityTask
{
	GENERATED_BODY()
public:
	
	//Create Async Node That Means Every Properties that made in this will attach to  CreateTargetDataUnderMouse functions
	UFUNCTION(BlueprintCallable,
			Category="Ability|Tasks", 
			meta=(DisplayName="TargetDataUnderLineTrace",
			HidePin="OwningAbility",
			DefaultToSelf="OwningAbility",
			BlueprintInternalUseOnly="true" )
			)
	static UTargetDataUnderLineTrace* TargetDataUnderLineTrace(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FLineTraceDelegate OnLineTraceComplete;

private:
	virtual void Activate() override;

	// The line trace logic
	void PerformLineTrace();

	FVector StartPoint;
	FVector EndPoint;
	ECollisionChannel Channel;

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,FGameplayTag ActivationTag);	
};
