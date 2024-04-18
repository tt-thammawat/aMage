// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "AMage_PhyMats.generated.h"

/**
 * 
 */
UCLASS()
class AMAGE_API UAMage_PhyMats : public UPhysicalMaterial
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=PhysicalMaterial)
	TObjectPtr<USoundBase> FootStepSound = nullptr;
};
