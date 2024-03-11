// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/ChargeProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "aMage/aMage.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AChargeProjectile::AChargeProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates=true;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetCollisionObjectType(ECC_PROJECTILE);
	CapsuleComponent->SetupAttachment(RootComponent);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	CapsuleComponent->SetupAttachment(RootComponent);
	RootComponent = CapsuleComponent;

	ProjectileMovementComponent =CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	// Ensure the projectile movement component updates its position based on the root component
	ProjectileMovementComponent->UpdatedComponent = CapsuleComponent;
}

void AChargeProjectile::Destroyed()
{
	//Client Run This Function // Will Get Effect Regardless
	if(!bHit && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
	}
	Super::Destroyed();
}

void AChargeProjectile::BeginPlay()
{
	Super::BeginPlay();
	//Set Life For This Actor
	SetLifeSpan(LifeSpan);
	
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnSphereOverlap);

	//Attached Sound For Looping
	UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent(),NAME_None,FVector(ForceInit),FRotator::ZeroRotator,EAttachLocation::KeepRelativeOffset,true);
}

void AChargeProjectile::OnSphereOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
	
	//Server Side
	if(HasAuthority())
	{
		//Get ASC By Using UAbilitySystemBlueprintLibrary Pass On Actor That We Want TO Do The Damage
		if(UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			//ApplyGameplayEffectSpecToThe OtherActor
			TargetAsc->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
	}
	else // Set This For Client
	{
		bHit = true;
	}
}


