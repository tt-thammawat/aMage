// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/MainProjectile.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/MainAbilitySystemLibrary.h"
#include "aMage/aMage.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainProjectile::AMainProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates=true;
	
	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRootComponent"));
	CustomRootComponent->SetupAttachment(RootComponent);
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetCollisionObjectType(ECC_PROJECTILE);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	CapsuleComponent->SetupAttachment(CustomRootComponent);
	
	ProjectileMovementComponent =CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	// Ensure the projectile movement component updates its position based on the root component
	ProjectileMovementComponent->UpdatedComponent = CustomRootComponent;
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
	if(!DamageEffectSpecHandle.Data.IsValid() || DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
	{
		return;
	}
	
	if(!UMainAbilitySystemLibrary::IsNotFriend(DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser(),OtherActor))
	{
		return;
	}
	
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
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AMainProjectile::DelayedDestroy);
	}
	else // Set This For Client
	{
		bHit = true;
	}
}

void AMainProjectile::DelayedDestroy()
{
	Destroy();
}

void AMainProjectile::BeginPlay()
{
	Super::BeginPlay();
	//Set Life For This Actor
	SetLifeSpan(LifeSpan);
	
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnSphereOverlap);

	//Attached Sound For Looping
	UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent(),NAME_None,FVector(ForceInit),FRotator::ZeroRotator,EAttachLocation::KeepRelativeOffset,true);
}




