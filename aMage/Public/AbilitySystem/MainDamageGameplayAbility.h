// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/MainGenericGameplayAbility.h"
#include "MainDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AMAGE_API UMainDamageGameplayAbility : public UMainGenericGameplayAbility
{
	GENERATED_BODY()
public:
	//Do Damage By Caller
	UFUNCTION(BlueprintCallable)
	void CauseDamages(AActor* TargetActor);
	
protected:
	
	//For UGameplayEffectExecutionCalculation
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag,FScalableFloat> DamageTypes;
};
