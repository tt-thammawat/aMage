// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/MainDamageGameplayAbility.h"
#include "GA_SpellProjectileBase.generated.h"

class AMainProjectile;
/**
 * 
 */
UCLASS()
class AMAGE_API UGA_SpellProjectileBase : public UMainDamageGameplayAbility
{
	GENERATED_BODY()

public:

	UGA_SpellProjectileBase();
	
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	UFUNCTION(BlueprintCallable,Category=Socket)
	FVector GetSocketLocation();

protected:
	UFUNCTION(BlueprintCallable,Category=Projectile)
	virtual void SpawnProjectile(const FVector& ProjectileTargetLocation,const FVector& CharSocketLocation, int32 NumProjectiles);
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AMainProjectile> ProjectileClass;

	
	
};
