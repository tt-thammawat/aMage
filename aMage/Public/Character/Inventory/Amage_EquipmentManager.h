// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AGR_EquipmentManager.h"
#include "Amage_EquipmentManager.generated.h"

class AMainItemInteractActor;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSwappedStaffSignature);
UCLASS()
class AMAGE_API UAmage_EquipmentManager : public UAGR_EquipmentManager
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Client,Reliable,BlueprintCallable)
	virtual void ClientUpdatedSwappedStaffUI();
	UPROPERTY(BlueprintAssignable)
	FOnSwappedStaffSignature OnClientUpdateSwappedStaff;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category="AGR|Game Play")
	int32 CurrentlyEquipIndex;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category="AGR|Game Play")
	int32 PreviousEquipIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category="AGR|Game Play")
	TSubclassOf<AMainItemInteractActor> DefaultStaff; 
};
