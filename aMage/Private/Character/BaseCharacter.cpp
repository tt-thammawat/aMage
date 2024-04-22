// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

#include "GameplayTagsSingleton.h"
#include "aMage/aMage.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "ActorComponents/FootStepsComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_PROJECTILE,ECR_Overlap);
	//Important To Get Overlap Event
	GetMesh()->SetGenerateOverlapEvents(true);
	
	TargetSceneComponent = CreateDefaultSubobject<USceneComponent>("TargetSceneComponent");
	TargetSceneComponent->SetupAttachment(GetMesh());
	
	Weapon=CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FootStepsComponent = CreateDefaultSubobject<UFootStepsComponent>("FootStepComp");
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(CapsuleTimerHandle, this, &ABaseCharacter::UpdateCapsuleAndRecoverLocation, 0.5f, true);
}

void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABaseCharacter::UpdateCapsuleAndRecoverLocation()
{
	if(bDead) return;
	if(!GetMesh()->IsSimulatingPhysics()) return;

	if(HasAuthority())
	{
		if (GetMesh() && GetMesh()->IsSimulatingPhysics())
		{
			if (GetMesh()->GetComponentVelocity().IsNearlyZero(10.f))
			{
				MulticastHandleUpdateCapsuleAndRecoverLocation();
			}
		}
	}
}

void ABaseCharacter::MulticastHandleUpdateCapsuleAndRecoverLocation_Implementation()
{
	FHitResult HitResult;
	FVector Start = GetMesh()->GetSocketLocation(TEXT("root"));
	FVector End = Start - FVector(0,0,-100.f);
	FVector TargetGroundLocation = Start;
	if(GetWorld()->LineTraceSingleByChannel(HitResult,Start,End,ECC_Visibility))
	{
		TargetGroundLocation = HitResult.Location;
	}
	
	GetCapsuleComponent()->SetWorldLocation(TargetGroundLocation + FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->PutAllRigidBodiesToSleep();
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	if(RecoverAnimMontage)
	{
		float MontageDuration = PlayAnimMontage(RecoverAnimMontage, 1);
	
		if (MontageDuration > 0.0f)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				// Bind the delegate to the OnMontageEnded event
				FOnMontageEnded MontageEndedDelegate;
				MontageEndedDelegate.BindUObject(this, &ABaseCharacter::OnRecoverMontageEnded);
				AnimInstance->Montage_SetEndDelegate( MontageEndedDelegate, RecoverAnimMontage);
			}
		}
	}
}

void ABaseCharacter::OnRecoverMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Vehicle,ECR_Block);
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

}

void ABaseCharacter::Die(const AActor* InstigatorActor)
{
	const FDetachmentTransformRules TransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld,true);
	Weapon->DetachFromComponent(TransformRules);
	
	OnDeath.Broadcast();
	
	MulticastHandleDeath();
}

void ABaseCharacter::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	bDead = true;
}

void ABaseCharacter::Dissolve()
{
	if(IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance,this);
		GetMesh()->SetMaterial(0,DynamicMatInst);

		StartDissolveTimeline(DynamicMatInst);
	}
	if(IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance,this);
		Weapon->SetMaterial(0,DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}

UAnimMontage* ABaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

bool ABaseCharacter::IsDead_Implementation() const
{
	return bDead;
}

AActor* ABaseCharacter::GetAvatar_Implementation()
{
	return this;
}

USceneComponent* ABaseCharacter::GetSceneComponent_Implementation()
{
	return TargetSceneComponent;
}

TArray<FTaggedMontage> ABaseCharacter::GetAttackMontage_Implementation()
{
	return AttackMontage;
}

//Override by derived
void ABaseCharacter::InitAbilityActorInfo()
{
	
}

FVector ABaseCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	FName* SocketNamePtr = MappedSocketName.Find(MontageTag);
	if (SocketNamePtr)
	{
		const FName SocketName = *SocketNamePtr;
		if (IsValid(Weapon) && MontageTag.MatchesTagExact(FMainGameplayTags::Get().Montage_Attack_Weapon))
		{
			return Weapon->GetSocketLocation(SocketName);
		}
		else
		{
			return GetMesh()->GetSocketLocation(SocketName);
		}
	}

	return FVector();
}

void ABaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	//Remember To AddSourceObject
	ContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass,1.f,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void ABaseCharacter::InitDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes,1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes,1.f);
	ApplyEffectToSelf(DefaultVitalAttributes,1.f);
}

void ABaseCharacter::AddCharacterAbilities()
{
	UBaseAbilitySystemComponent* BaseAbilitySystemComponent = CastChecked<UBaseAbilitySystemComponent>(AbilitySystemComponent);

	if(!HasAuthority()) return;

	BaseAbilitySystemComponent->AddCharacterAbilities(StartUpAbilities,nullptr);

}


//return this ASC
UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;

}



