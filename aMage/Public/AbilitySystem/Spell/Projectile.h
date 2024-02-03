// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USoundCue;
class UProjectileMovementComponent;
class UBoxComponent;

UCLASS()
class AMAGE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Destroyed() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
		);

private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Collision , meta=(AllowPrivateAccess=true));
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere,meta=(AllowPrivateAccess=true));
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> Tracer;
	
	TObjectPtr<UParticleSystemComponent> TracerComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ImpactParticle;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> ImpactSound;
public:
	UBoxComponent* GetBoxComponent() const {return BoxComponent;};
};
