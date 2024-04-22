// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InteractActor/MainItemInteractActor.h"
#include "Character/Inventory/MainAbilitiesItemComponent.h"
#include "Net/UnrealNetwork.h"

AMainItemInteractActor::AMainItemInteractActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates=true;

	MainAbilitiesItemComponent = CreateDefaultSubobject<UMainAbilitiesItemComponent>("MainAbilitiesItemComponent");

	ItemEquipMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ItemEquipMesh");
	ItemEquipMesh->SetupAttachment(CustomRootBug);
}

void AMainItemInteractActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,bIsPickup);
}

void AMainItemInteractActor::HighlightActor_Implementation()
{
	MulticastHighlight();
}

void AMainItemInteractActor::UnHighlightActor_Implementation()
{
	MulticastUnHighlight();
}

void AMainItemInteractActor::OnRep_bIsPickup()
{
	MulticastUnHighlight();
}

void AMainItemInteractActor::MulticastHighlight_Implementation()
{
	ItemEquipMesh->SetRenderCustomDepth(true);
	ItemEquipMesh->SetCustomDepthStencilValue(249);
}

void AMainItemInteractActor::MulticastUnHighlight_Implementation()
{
	ItemEquipMesh->SetRenderCustomDepth(false);

}
