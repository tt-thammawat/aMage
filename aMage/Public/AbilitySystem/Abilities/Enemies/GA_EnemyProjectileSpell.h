// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/MainDamageGameplayAbility.h"
#include "GA_EnemyProjectileSpell.generated.h"

class AMainProjectile;
/**
 * 
 */
UCLASS()
class AMAGE_API UGA_EnemyProjectileSpell : public UMainDamageGameplayAbility
{
	GENERATED_BODY()
public:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(BlueprintCallable,Category=Socket)
	FVector GetSocketLocation();

protected:
	UFUNCTION(BlueprintCallable,Category=Projectile)
	virtual void SpawnProjectile(const FVector& ProjectileTargetLocation,const FVector& CharSocketLocation, int32 NumProjectiles);
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AMainProjectile> ProjectileClass;
};
