// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Inventory/Amage_EquipmentManager.h"

#include "Net/UnrealNetwork.h"

void UAmage_EquipmentManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ThisClass,CurrentlyEquipIndex,COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ThisClass,PreviousEquipIndex,COND_OwnerOnly);

}

void UAmage_EquipmentManager::ClientUpdatedSwappedStaffUI_Implementation()
{
	OnClientUpdateSwappedStaff.Broadcast();
}


