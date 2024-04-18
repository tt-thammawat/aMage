// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "ChargeProjectile.generated.h"

class UCapsuleComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;


USTRUCT()
struct FEffectInActor
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AActor> Actor;
	
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandles;
};

UCLASS()
class AMAGE_API AChargeProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AChargeProjectile();
	virtual void Destroyed() override;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	//Handle UGameplayEffect
	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;
	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle EffectSpecHandle;
	UPROPERTY()
	TArray<FEffectInActor> ActiveEffectActors;
	
protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlapComponent,AActor* OtherActor , UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	virtual void OnEndOverlap(AActor* ActorToRemove);

	virtual void RemoveEffectsForActor(AActor* RemoveEffectTarget);
	
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	TObjectPtr<AActor> TargetActor;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	bool bRemoveEffectOnEnd=false;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> CustomRootComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditAnyWhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan;
	
	bool bHit=false;

public:
	void SetTargetActor(AActor* Actor) {TargetActor=Actor;};

};
