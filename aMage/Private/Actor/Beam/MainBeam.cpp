// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Beam/MainBeam.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "aMage/aMage.h"
#include "Components/SphereComponent.h"

AMainBeam::AMainBeam()
{
	PrimaryActorTick.bCanEverTick = false;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRootComponent"));
	CustomRootComponent->SetupAttachment(RootComponent);
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionObjectType(ECC_PROJECTILE);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	
	BeamNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BeamNiagara"));

}


void AMainBeam::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnSphereOverlap);

}

void AMainBeam::ActivateBeamAndSetBeam(FVector BeamEndLocation, FVector BeamStartLocation)
{
	// Enable SphereComponent and move it to the end location of the beam
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetWorldLocation(BeamEndLocation);

	// Enable the Niagara component and set the beam start and end locations
	if (BeamNiagara)
	{
		BeamNiagara->SetVisibility(true);
		BeamNiagara->Activate(true);

		// Assuming your Niagara system has parameters named 'BeamStart' and 'BeamEnd'
		BeamNiagara->SetVariableVec3("Beam Start", BeamStartLocation);
		BeamNiagara->SetVariableVec3("Beam End", BeamEndLocation);
	}
}

void AMainBeam::DeactivateBeam()
{
	// Disable SphereComponent
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Disable the Niagara component
	if (BeamNiagara)
	{
		BeamNiagara->SetVisibility(false);
		BeamNiagara->Deactivate();
	}
}

void AMainBeam::OnSphereOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		if(UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			//ApplyGameplayEffectSpecToThe OtherActor
			TargetAsc->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
	}
}


