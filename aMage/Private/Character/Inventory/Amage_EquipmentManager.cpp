// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Inventory/Amage_EquipmentManager.h"

#include "Net/UnrealNetwork.h"

void UAmage_EquipmentManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,CurrentlyEquipIndex);
	DOREPLIFETIME(ThisClass,PreviousEquipIndex);

}

