// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "ChargeProjectile.generated.h"

class UCapsuleComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;

UCLASS()
class AMAGE_API AChargeProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AChargeProjectile();
	virtual void Destroyed() override;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	//Handle UGameplayEffect
	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;
	
protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlapComponent,AActor* OtherActor , UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

private:
	
	UPROPERTY(VisibleAnywhere)
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

};
