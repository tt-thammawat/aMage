// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/MainProjectile.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "aMage/aMage.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainProjectile::AMainProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates=true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
	SphereComponent->SetCollisionObjectType(ECC_PROJECTILE);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	ProjectileMovementComponent =CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void AMainProjectile::Destroyed()
{
	//Client Run This Function // Will Get Effect Regardless
	if(!bHit && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
	}
	Super::Destroyed();
}

void AMainProjectile::OnSphereOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
	
	//Destroy On The Server Side
	if(HasAuthority())
	{
		//Get ASC By Using UAbilitySystemBlueprintLibrary Pass On Actor That We Want TO Do The Damage
		if(UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			//ApplyGameplayEffectSpecToThe OtherActor
			TargetAsc->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		Destroy();
	}
	else // Set This For Client
	{
		bHit = true;
	}
}


void AMainProjectile::BeginPlay()
{
	Super::BeginPlay();
	//Set Life For This Actor
	SetLifeSpan(LifeSpan);
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnSphereOverlap);

	//Attached Sound For Looping
	UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent(),NAME_None,FVector(ForceInit),FRotator::ZeroRotator,EAttachLocation::KeepRelativeOffset,true);
}




