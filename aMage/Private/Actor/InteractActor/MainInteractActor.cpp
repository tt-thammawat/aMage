// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InteractActor/MainInteractActor.h"

#include "Components/WidgetComponent.h"

AMainInteractActor::AMainInteractActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates=true;

	CustomRootBug = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRootBug"));
	CustomRootBug->SetupAttachment(RootComponent);

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractWidget->SetupAttachment(CustomRootBug);
	InteractWidget->SetVisibility(false);

}

void AMainInteractActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainInteractActor::InteractWithItem(AActor* Actor)
{
	if (HasAuthority())
	{
		GetInteractActor(Actor);
	}
	else
	{
		// Call the server function from the client
		ServerGetInteractActor(Actor);
	}
}


void AMainInteractActor::ShowInteractDetail()
{
	InteractWidget->SetVisibility(true);
}

void AMainInteractActor::HideInteractDetail()
{
	InteractWidget->SetVisibility(false);

}

void AMainInteractActor::ServerGetInteractActor_Implementation(AActor* Actor)
{
	GetInteractActor(Actor);
}
