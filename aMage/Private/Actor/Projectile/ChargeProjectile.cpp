// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/ChargeProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/MainAbilitySystemLibrary.h"
#include "aMage/aMage.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AChargeProjectile::AChargeProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates=true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRootComponent"));
	CustomRootComponent->SetupAttachment(RootComponent);
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetCollisionObjectType(ECC_PROJECTILE);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	CapsuleComponent->SetupAttachment(CustomRootComponent);
	
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
	
	//Server Side
	if (HasAuthority())
		if (UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			FActiveGameplayEffectHandle NewDamageEffectHandle = TargetAsc->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
    
			FEffectInActor* ActorEffects = ActiveEffectActors.FindByPredicate([&](const FEffectInActor& Item)
			{
				return Item.Actor == OtherActor;
			});

			if (!ActorEffects)
			{
				FEffectInActor NewActorEffects;
				NewActorEffects.Actor = OtherActor;
				ActiveEffectActors.Add(NewActorEffects);
				ActorEffects = &ActiveEffectActors.Last();
			}

			ActorEffects->ActiveGameplayEffectHandles.AddUnique(NewDamageEffectHandle);

			if (EffectSpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle NewEffectHandle = TargetAsc->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
				ActorEffects->ActiveGameplayEffectHandles.AddUnique(NewEffectHandle);
			}
		}
	else
	{
		bHit = true;
	}
}

void AChargeProjectile::OnEndOverlap(AActor* ActorToRemove)
{
	int32 Index = ActiveEffectActors.IndexOfByPredicate([&](const FEffectInActor& EffectActor)
	{
		return EffectActor.Actor == ActorToRemove;
	});

	if (Index != INDEX_NONE && bRemoveEffectOnEnd)
	{
		RemoveEffectsForActor(ActorToRemove);
	}
}

void AChargeProjectile::RemoveEffectsForActor(AActor* RemoveEffectTarget)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(RemoveEffectTarget);

	if (!IsValid(TargetASC)) return;

	// Find the FEffectInActor structure for RemoveEffectTarget
	FEffectInActor* EffectActor = ActiveEffectActors.FindByPredicate([&](const FEffectInActor& Item)
	{
		return Item.Actor == RemoveEffectTarget;
	});

	if (EffectActor)
	{
		// Iterate through all effect handles and remove them
		for (FActiveGameplayEffectHandle& Handle : EffectActor->ActiveGameplayEffectHandles)
		{
			if (Handle.IsValid())
			{
				TargetASC->RemoveActiveGameplayEffect(Handle, INT32_MAX);
			}
		}
		// Remove the FEffectInActor from the array after processing
		ActiveEffectActors.RemoveAll([&](const FEffectInActor& Item)
		{
			return Item.Actor == RemoveEffectTarget;
		});
	}
}


