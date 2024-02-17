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
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	virtual void ActivateAbilityAfterHold();

protected:

	UPROPERTY(Replicated,VisibleAnywhere,BlueprintReadWrite)
	bool bIsPlayingAnimation;
	
	UFUNCTION(BlueprintCallable,Category=Projectile)
	virtual void SpawnChargeProjectile(const FVector& ProjectileTargetLocation, int32 NumProjectiles);
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AChargeProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	FTimerHandle TimerHandle_InputHeld;
	float InputPressTime = 0.0f;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Charge)
	float InputHeldDuration = 3.0f; // Hold duration in seconds
};


