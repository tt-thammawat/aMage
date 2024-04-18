// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/FootStepsComponent.h"
#include "Character/MainPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMats/AMage_PhyMats.h"

UFootStepsComponent::UFootStepsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UFootStepsComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UFootStepsComponent::HandleFootStep(EFoot Foot)
{
	if(ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
	{
		if(USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh())
		{
			FHitResult HitResult;
			const FVector SocketLocation = SkeletalMeshComponent->GetSocketLocation(Foot == EFoot::ELeft?LeftFootSocketName:RightFootSocketName);
			const FVector Location = SocketLocation + FVector::UpVector + 20;

			FCollisionQueryParams QueryParams;
			QueryParams.bReturnPhysicalMaterial = true;
			QueryParams.AddIgnoredActor(Character);

			if(GetWorld()->LineTraceSingleByChannel(HitResult,Location,Location + FVector::UpVector * -50.f,ECC_WorldStatic,QueryParams))
			{
				if(HitResult.bBlockingHit)
				{
					if(HitResult.PhysMaterial.Get())
					{
						UAMage_PhyMats* AMage_PhyMats = Cast<UAMage_PhyMats>(HitResult.PhysMaterial.Get());
						if(AMage_PhyMats)
						{
							UGameplayStatics::PlaySoundAtLocation(this,AMage_PhyMats->FootStepSound,Location,1.f);
						}
						
					}
				}
			}
		}
	}
}



