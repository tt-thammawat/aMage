// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "MainBeam.generated.h"

class UNiagaraComponent;
class USphereComponent;

UCLASS()
class AMAGE_API AMainBeam : public AActor
{
	GENERATED_BODY()
	
public:	
	AMainBeam();
	
	UFUNCTION(BlueprintCallable,Category=Beam)
	void ActivateBeamAndSetBeam(FVector BeamEndLocation,FVector BeamStartLocation);
	UFUNCTION(Server,Reliable,Category=Beam)
	void ServerRequestActivateBeam(FVector BeamEndLocation, FVector BeamStartLocation);
	UFUNCTION(NetMulticast,Reliable,Category=Beam)
	void NetMulticastActivateBeamAndSetBeam(FVector BeamEndLocation, FVector BeamStartLocation);

	
	UFUNCTION(BlueprintCallable,Category=Beam)
	void DeactivateBeam();
	UFUNCTION(Server,Reliable,Category=Beam)
	void ServerDeactivateBeam();
	UFUNCTION(NetMulticast,Reliable,Category=Beam)
	void MulticastDeactivateBeam();
	
	//HandleThisDamageEffect
	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	FActiveGameplayEffectHandle ActiveDamageEffect;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	virtual void OnEndOverlap(AActor* TargetActor);

	virtual void RemoveSingleEffect(AActor* RemoveEffectTarget);

private:
	UPROPERTY(VisibleAnywhere,Category="Default")
	TObjectPtr<USceneComponent> CustomRootComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> BeamNiagara;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;
	
};
