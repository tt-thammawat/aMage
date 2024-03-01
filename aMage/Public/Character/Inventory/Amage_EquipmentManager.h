// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AGR_EquipmentManager.h"
#include "Amage_EquipmentManager.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentListUpdated, const TArray<FEquipment>&, EquipmentList);

UCLASS()
class AMAGE_API UAmage_EquipmentManager : public UAGR_EquipmentManager
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category="AGR|Game Play")
	int32 CurrentlyEquipIndex;
};
