// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InteractActor/MainInteractActor.h"

#include "Components/WidgetComponent.h"

AMainInteractActor::AMainInteractActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRootComponent"));
	CustomRootComponent->SetupAttachment(RootComponent);
	
	InteractWidget = CreateDefaultSubobject<UWidgetComponent>("InteractWidget");
	InteractWidget->SetupAttachment(CustomRootComponent);
	InteractWidget->SetVisibility(false);
	SetReplicates(true);
}

void AMainInteractActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainInteractActor::InteractWithItem(AActor* Actor)
{
	GetInteractActor(Actor);
}


void AMainInteractActor::ShowInteractDetail()
{
	InteractWidget->SetVisibility(true);
}

void AMainInteractActor::HideInteractDetail()
{
	InteractWidget->SetVisibility(false);
}
