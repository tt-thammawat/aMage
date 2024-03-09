// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RuneSpell/Projectile/GA_SpellProjectileBase.h"
#include "GA_SpellChargeProjectile.generated.h"

class AChargeProjectile;
/**
 * 
 */
UCLASS()
class AMAGE_API UGA_SpellChargeProjectile : public UMainGenericGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_SpellChargeProjectile();
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	
	UFUNCTION()
	virtual void UpdateValueDuringHold();
	UFUNCTION(BlueprintImplementableEvent)
	void GetValueWhenPressed(float PressedTimes);
protected:
	
	UFUNCTION(BlueprintCallable,Category=Projectile)
	virtual void SpawnChargeProjectile(const FVector& ProjectileTargetLocation, int32 NumProjectiles);
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AChargeProjectile> ProjectileClass;
	UPROPERTY()
	FTimerHandle TimerHandle_UpdateHeldValue;
};


