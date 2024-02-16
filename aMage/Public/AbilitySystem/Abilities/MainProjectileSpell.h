// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MainGameplayAbility.h"
#include "MainProjectileSpell.generated.h"

class AMainProjectile;
/**
 * 
 */
UCLASS()
class AMAGE_API UMainProjectileSpell : public UMainGameplayAbility
{
	GENERATED_BODY()
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(BlueprintCallable,Category=Projectile)
	virtual void SpawnProjectile(const FVector& ProjectileTargetLocation);

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AMainProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;


};
