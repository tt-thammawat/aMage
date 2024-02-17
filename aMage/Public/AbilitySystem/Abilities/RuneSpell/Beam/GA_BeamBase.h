// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MainGenericGameplayAbility.h"
#include "GA_BeamBase.generated.h"

class UNiagaraComponent;
class AMainBeam;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class AMAGE_API UGA_BeamBase : public UMainGenericGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_BeamBase();
	
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void RepeatedlyActivateAbility();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//Beam
	UFUNCTION(BlueprintCallable,Category=Beam)
	FVector GetSocketLocation();
	
protected:
	FTimerHandle AbilityActivationTimer;
	
	bool bIsInputHeld=false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AMainBeam> BeamClass;
	
	UFUNCTION(BlueprintCallable,Category=Beam)
	virtual void SpawnBeam(const FVector& BeamEndLocation);
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<AMainBeam> BeamREF;
		
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	

};
