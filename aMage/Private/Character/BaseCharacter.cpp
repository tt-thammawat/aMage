// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

#include "GameplayTagsSingleton.h"
#include "aMage/aMage.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_PROJECTILE,ECR_Overlap);
	//Important To Get Overlap Event
	GetMesh()->SetGenerateOverlapEvents(true);
	
	Weapon=CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}


void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
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

//return this ASC
UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;

}

void ABaseCharacter::Die()
{
	FDetachmentTransformRules TransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld,true);
	Weapon->DetachFromComponent(TransformRules);
	
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
		if (IsValid(Weapon))
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




