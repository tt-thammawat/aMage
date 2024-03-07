// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MainGameplayAbility.h"
#include "MainGenericGameplayAbility.generated.h"

class UMainPlayerWidget;
class UMainInputAction;
/**
 * 
 */
UCLASS()
class AMAGE_API UMainGenericGameplayAbility : public UMainGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
		
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION(BlueprintCallable)
	void RemoveAbilityAfterEnd(const TArray<TSubclassOf<UGameplayAbility>>& RemoveAbilities);
	
	//Do Damage By Caller
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Spell)
	TSubclassOf<UMainPlayerWidget> SpellIndicatorClass;

	TObjectPtr<UMainPlayerWidget> SpellIndicator;
	
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category=Spell)
	float UsageTimes;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category=Spell)
	bool bIsCancel=false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	//For UGameplayEffectExecutionCalculation
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag,FScalableFloat> DamageType;

};
