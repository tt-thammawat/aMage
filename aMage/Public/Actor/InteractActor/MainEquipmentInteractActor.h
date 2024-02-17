// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/InteractActor/MainItemInteractActor.h"
#include "MainEquipmentInteractActor.generated.h"

/**
 * 
 */
UCLASS()
class AMAGE_API AMainEquipmentInteractActor : public AMainItemInteractActor
{
	GENERATED_BODY()
public:
	AMainEquipmentInteractActor();
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Weapon)
	TObjectPtr<USkeletalMeshComponent> WeaponEquipMesh;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Weapon)
	FName WeaponTipSocketName;
public:
	USkeletalMeshComponent* GetSkeletalMeshComponent() const {return WeaponEquipMesh;};
	FORCEINLINE FName GetWeaponTipSocketName() const {return WeaponTipSocketName;};
};
