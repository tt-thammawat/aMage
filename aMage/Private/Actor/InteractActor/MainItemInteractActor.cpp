// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InteractActor/MainItemInteractActor.h"
#include "Character/Inventory/MainAbilitiesItemComponent.h"

AMainItemInteractActor::AMainItemInteractActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates=true;

	MainAbilitiesItemComponent = CreateDefaultSubobject<UMainAbilitiesItemComponent>("MainAbilitiesItemComponent");

	ItemEquipMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ItemEquipMesh");
	ItemEquipMesh->SetupAttachment(CustomRootBug);
}
