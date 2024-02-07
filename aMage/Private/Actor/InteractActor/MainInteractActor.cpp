// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InteractActor/MainInteractActor.h"

#include "Components/WidgetComponent.h"

AMainInteractActor::AMainInteractActor()
{
	PrimaryActorTick.bCanEverTick = false;
	InteractWidget = CreateDefaultSubobject<UWidgetComponent>("InteractWidget");
	InteractWidget->SetupAttachment(RootComponent);
	InteractWidget->SetVisibility(false);
}

void AMainInteractActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainInteractActor::InteractWithItem()
{
	
}

void AMainInteractActor::ShowInteractDetail()
{
	InteractWidget->SetVisibility(true);
}

void AMainInteractActor::HideInteractDetail()
{
	InteractWidget->SetVisibility(false);
}


