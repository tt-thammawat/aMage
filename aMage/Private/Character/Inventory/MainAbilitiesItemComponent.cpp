// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Inventory/MainAbilitiesItemComponent.h"

#include "Net/UnrealNetwork.h"

void UMainAbilitiesItemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, ItemIcon);
	DOREPLIFETIME(ThisClass, ItemDescription);
	DOREPLIFETIME(ThisClass, BaseAbilityLMBAction);
	
	DOREPLIFETIME(ThisClass, MatchingInfo);
	DOREPLIFETIME(ThisClass, AbilitiesDescriptions);
	DOREPLIFETIME(ThisClass, UsageTimes);
	DOREPLIFETIME(ThisClass, SpecialAbilityLMBAction);
	DOREPLIFETIME(ThisClass, SpecialAbilityRMBAction);
	
	DOREPLIFETIME(ThisClass, ItemAbilities);

}
