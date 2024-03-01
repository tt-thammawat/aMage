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

	bReplicates = true;
	
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
	BeamNiagara->SetIsReplicated(true);
}

void AMainBeam::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMainBeam::OnOverlap(AActor* TargetActor)
{
		if(UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
		{
			//ApplyGameplayEffectSpecToThe OtherActor
			ActiveDamageEffect=TargetAsc->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
}

void AMainBeam::OnEndOverlap(AActor* TargetActor)
{
	if(ActiveDamageEffect.IsValid())
	{
		RemoveSingleEffect(TargetActor);
	}
}

void AMainBeam::RemoveSingleEffect(AActor* RemoveEffectTarget)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(RemoveEffectTarget);

	if (!IsValid(TargetASC)) return;

	if (ActiveDamageEffect.IsValid())
	{
		TargetASC->RemoveActiveGameplayEffect(ActiveDamageEffect, INT32_MAX);
		ActiveDamageEffect.Invalidate(); 
	}
}

void AMainBeam::ActivateBeamAndSetBeam(FVector BeamEndLocation, FVector BeamStartLocation)
{
	if (HasAuthority())
	{
		NetMulticastActivateBeamAndSetBeam(BeamEndLocation,BeamStartLocation);
	}
	else
	{
		ServerRequestActivateBeam(BeamEndLocation,BeamStartLocation);
	}
}

void AMainBeam::ServerRequestActivateBeam_Implementation(FVector BeamEndLocation, FVector BeamStartLocation)
{
	NetMulticastActivateBeamAndSetBeam(BeamEndLocation, BeamStartLocation);
}

void AMainBeam::NetMulticastActivateBeamAndSetBeam_Implementation(FVector BeamEndLocation, FVector BeamStartLocation)
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
	if(HasAuthority())
	{
		MulticastDeactivateBeam();
	}
	else
	{
		ServerDeactivateBeam();
	}
}

void AMainBeam::ServerDeactivateBeam_Implementation()
{
	MulticastDeactivateBeam();
}

void AMainBeam::MulticastDeactivateBeam_Implementation()
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



