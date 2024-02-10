// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AGR_EquipmentManager.h"
#include "Amage_EquipmentManager.generated.h"

/**
 * 
 */
UCLASS()
class AMAGE_API UAmage_EquipmentManager : public UAGR_EquipmentManager
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category="AGR|Game Play")
	FName CurrentlyEquipSlot;
};
