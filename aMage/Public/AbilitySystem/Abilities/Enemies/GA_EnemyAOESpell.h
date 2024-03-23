// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/MainDamageGameplayAbility.h"
#include "GA_EnemyAOESpell.generated.h"

class AChargeProjectile;
/**
 * 
 */
UCLASS()
class AMAGE_API UGA_EnemyAOESpell : public UMainDamageGameplayAbility
{
	GENERATED_BODY()
public:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION(BlueprintCallable,Category=Socket)
	FVector GetSocketLocation();

protected:
	UFUNCTION(BlueprintCallable,Category=AOE)
	virtual void SpawnAOE(AActor* PlayerActor);
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<AChargeProjectile> AOERef;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AChargeProjectile> AOEClass;
};
