// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/MainGameplayAbility.h"
#include "Interact/ICombatInterface.h"
#include "MainDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AMAGE_API UMainDamageGameplayAbility : public UMainGameplayAbility
{
	GENERATED_BODY()
public:
	//Do Damage By Caller
	UFUNCTION(BlueprintCallable)
	void CauseDamages(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
protected:
	
	//For UGameplayEffectExecutionCalculation
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag,FScalableFloat> DamageTypes;

	//Effect That Send To Projectile
	//Effect Damage Reduce Health
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	//Effect That Can be remove when damage is done
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Damage")
	TSubclassOf<UGameplayEffect> EffectClass;
};
