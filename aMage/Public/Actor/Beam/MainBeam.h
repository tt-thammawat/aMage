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

	UFUNCTION(BlueprintCallable,Category=Beam)
	void DeactivateBeam();

	//HandleThisDamageEffect
	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlapComponent,AActor* OtherActor , UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

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
