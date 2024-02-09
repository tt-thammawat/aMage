// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InteractActor/MainItemInteractActor.h"
#include "Character/Inventory/MainAbilitiesItemComponent.h"

AMainItemInteractActor::AMainItemInteractActor()
{
	AbilitiesItemComponent = CreateDefaultSubobject<UMainAbilitiesItemComponent>("AbilitiesItemComponent");
	
}
