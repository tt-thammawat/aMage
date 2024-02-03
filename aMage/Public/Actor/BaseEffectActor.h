// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "BaseEffectActor.generated.h"

class UWidgetComponent;
class UEffectDataAsset;
class UAbilitySystemComponent;
//Manage/Remove GameplayEffect
struct FActiveGameplayEffectHandle;
class UGameplayEffect;

UCLASS()
class AMAGE_API ABaseEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void ApplyEffectToTarget(AActor* TargetActor,TSubclassOf<UGameplayEffect> GameplayEffectClass,float& ActorLevel);
	
	UFUNCTION(BlueprintCallable)
	virtual void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	virtual void OnEndOverlap(AActor* TargetActor);

	virtual void RemoveSingleEffect(AActor* RemoveEffectTarget);

protected:
	
	//Data Assets Effect Store
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TArray<UEffectDataAsset*> EffectDataAssets;
	
	//TMap can handle 2 identifies Handle For Store And Delete Effect
	TMap<FActiveGameplayEffectHandle,UAbilitySystemComponent*> ActiveEffectHandles;
	
	
	
};
