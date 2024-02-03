// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PickUpEffectActor.h"

#include "Character/MainPlayerCharacter.h"
#include "Components/WidgetComponent.h"


APickUpEffectActor::APickUpEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	PickUpSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("PickUpSkeletalMeshComponent");
	PickUpSkeletalMeshComponent->SetupAttachment(RootComponent);
	PickUpWidget = CreateDefaultSubobject<UWidgetComponent>("PickUpWidget");
	PickUpWidget->SetupAttachment(PickUpSkeletalMeshComponent);
	PickUpWidget->SetVisibility(false);
	PickUpSkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	PickUpSkeletalMeshComponent->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	PickUpSkeletalMeshComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	PickUpSkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
}

void APickUpEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickUpEffectActor::OnInteractWithActor(AActor* TargetActor)
{
	if(TargetActor->HasAuthority())
	{
		IInteractInterface* InteractCharacter = Cast<IInteractInterface>(TargetActor);
		InteractCharacter->InteractWithItem(this);
	}
}

void APickUpEffectActor::OnEndInteractWithActor(AActor* TargetActor)
{
	if(TargetActor->HasAuthority())
	{
		IInteractInterface* InteractCharacter = Cast<IInteractInterface>(TargetActor);
		InteractCharacter->InteractWithItem(nullptr);
	}
}


void APickUpEffectActor::DestroyAfterPickUp(AActor* TargetActor)
{
	OnOverlap(TargetActor);
	Destroy();
}


void APickUpEffectActor::ShowDetail()
{
	PickUpWidget->SetVisibility(true);

}

void APickUpEffectActor::UnShowDetail()
{
	
	PickUpWidget->SetVisibility(false);

}



