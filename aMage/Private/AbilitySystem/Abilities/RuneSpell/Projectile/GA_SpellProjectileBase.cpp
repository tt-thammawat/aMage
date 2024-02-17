// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/RuneSpell/Projectile/GA_SpellProjectileBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/Projectile/MainProjectile.h"
#include "Character/MainPlayerCharacter.h"
#include "Interact/ICombatInterface.h"
#include "Net/UnrealNetwork.h"


UGA_SpellProjectileBase::UGA_SpellProjectileBase() :
bIsPlayingAnimation(false)
{
	NetExecutionPolicy=EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy=EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_SpellProjectileBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(UGA_SpellProjectileBase,bIsPlayingAnimation,COND_OwnerOnly);

}

bool UGA_SpellProjectileBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                 const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_SpellProjectileBase::InputPressed(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if(!bIsPlayingAnimation)
	{
		Super::InputPressed(Handle, ActorInfo, ActivationInfo);
		K2_ActivateAbility();
	}
	
}

void UGA_SpellProjectileBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMainPlayerCharacter* MainPlayerCharacter = CastChecked<AMainPlayerCharacter>(ActorInfo->AvatarActor.Get(),ECastCheckedType::NullAllowed);
	
	MainPlayerCharacter->SetIsAiming(true);
}

void UGA_SpellProjectileBase::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

void UGA_SpellProjectileBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	AMainPlayerCharacter* MainPlayerCharacter = CastChecked<AMainPlayerCharacter>(ActorInfo->AvatarActor.Get(),ECastCheckedType::NullAllowed);

	MainPlayerCharacter->SetIsAiming(false);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_SpellProjectileBase::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	AMainPlayerCharacter* MainPlayerCharacter = CastChecked<AMainPlayerCharacter>(ActorInfo->AvatarActor.Get(),ECastCheckedType::NullAllowed);

	MainPlayerCharacter->SetIsAiming(false);
	
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_SpellProjectileBase::SpawnProjectile(const FVector& ProjectileTargetLocation, int32 NumProjectiles)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer) return;
	//Get Avatar Actor That Cast This
	IICombatInterface* CombatInterface = Cast<IICombatInterface>(GetAvatarActorFromActorInfo());
	if(CombatInterface)
	{
		// Get SocketLocation FVector via ICombatInterface
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

		// Adjust the angle based on the number of projectiles
		float AngleOffset;
		if (NumProjectiles == 3)
		{
			AngleOffset = 10.0f; // Example angle, adjust as needed
		}
		else if (NumProjectiles == 5)
		{
			AngleOffset = 15.0f; // Example angle, adjust as needed
		}
		else
		{
			AngleOffset = 0.0f; // Single projectile goes straight
		}

		for(int32 i = 0; i < NumProjectiles; ++i)
		{
			// Calculate rotation for each projectile
			float RotationOffset = AngleOffset * (i - NumProjectiles / 2);
			FRotator SpawnRotation = Rotation + FRotator(0.0f, RotationOffset, 0.0f);
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SocketLocation);
			SpawnTransform.SetRotation(SpawnRotation.Quaternion());
			
			//It gives developers the opportunity to modify or configure the newly spawned actor or its components before the actor's construction scripts and initialization are fully completed.
			AMainProjectile* Projectile = GetWorld()->SpawnActorDeferred<AMainProjectile>(
					ProjectileClass,
					SpawnTransform,
					GetOwningActorFromActorInfo(),
					Cast<APawn>(GetOwningActorFromActorInfo()),
					ESpawnActorCollisionHandlingMethod::AlwaysSpawn
					);

			Projectile->SetOwner(GetOwningActorFromActorInfo());
			//Give The Projectile A Gameplay Effect Spec For Causing Damage
			//Get AbilitySystemComponent
			const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
			//Make Spec Handle That Contains Effect Information And Send It To Projectile
			const FGameplayEffectSpecHandle SpecHandle= SourceASC->MakeOutgoingSpec(DamageEffectClass,GetAbilityLevel(),SourceASC->MakeEffectContext());
			//Now The Projectile Have The Damage From This Spell
			Projectile->DamageEffectSpecHandle=SpecHandle;
			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}
