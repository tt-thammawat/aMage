// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/RuneSpell/Projectile/GA_SpellChargeProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsSingleton.h"
#include "Actor/Projectile/ChargeProjectile.h"
#include "Character/MainPlayerCharacter.h"

UGA_SpellChargeProjectile::UGA_SpellChargeProjectile()
{
	NetExecutionPolicy=EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	InstancingPolicy=EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGA_SpellChargeProjectile::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_SpellChargeProjectile::InputPressed(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_UpdateHeldValue, this, &UGA_SpellChargeProjectile::UpdateValueDuringHold, 0.05f,true);
}

void UGA_SpellChargeProjectile::ActivateAbilityAfterHeld()
{
	// Used To Activated Ability Here But Now Move It To InputRelease
}

void UGA_SpellChargeProjectile::UpdateValueDuringHold()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_InputHeld))
	{
		RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_InputHeld);
		GetValueWhenPressed(RemainingTime);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_UpdateHeldValue);
		GetValueWhenPressed(0.f);
	}
}

void UGA_SpellChargeProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	MainPlayerCharacter->SetIsAiming(true);
}

void UGA_SpellChargeProjectile::SpawnChargeProjectile(const FVector& ProjectileTargetLocation, int32 NumProjectiles)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer) return;
	
		// Get SocketLocation FVector via ICombatInterface
		const FVector SocketLocation = 	IICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo());
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
			SpawnRotation.Pitch = 0;
			
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SocketLocation);
			SpawnTransform.SetRotation(SpawnRotation.Quaternion());
			
			AChargeProjectile* Projectile = GetWorld()->SpawnActorDeferred<AChargeProjectile>(
					ProjectileClass,
					SpawnTransform,
					GetOwningActorFromActorInfo(),
					Cast<APawn>(GetOwningActorFromActorInfo()),
					ESpawnActorCollisionHandlingMethod::AlwaysSpawn
					);

			//Give The Projectile A Gameplay Effect Spec For Causing Damage
			//Get AbilitySystemComponent
			const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
			FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
			//Set AbilityCDO //Add Those Things to check if information is pass along
			EffectContextHandle.SetAbility(this);
			EffectContextHandle.AddSourceObject(Projectile);
			TArray<TWeakObjectPtr<AActor>> Actors;
			Actors.Add(Projectile);
			EffectContextHandle.AddActors(Actors);
			FHitResult HitResult;
			HitResult.Location = ProjectileTargetLocation;
			EffectContextHandle.AddHitResult(HitResult);
		
			//Make Spec Handle That Contains Effect Information And Send It To Projectile
			const FGameplayEffectSpecHandle SpecHandle= SourceASC->MakeOutgoingSpec(DamageEffectClass,GetAbilityLevel(),EffectContextHandle);
			const FMainGameplayTags MainGameplayTags = FMainGameplayTags::Get();

			for (auto& Pair : DamageType)
			{
				const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,Pair.Key,ScaledDamage);
			}
		
			//Now The Projectile Have The Damage From This Spell
			Projectile->DamageEffectSpecHandle=SpecHandle;
			Projectile->FinishSpawning(SpawnTransform);
		}
}

void UGA_SpellChargeProjectile::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{

	MainPlayerCharacter->SetIsAiming(false);
	
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_SpellChargeProjectile::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_InputHeld))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_InputHeld);
	}

	// Check if the remaining time is within the threshold
	if (RemainingTime <= 0.1f)
	{
		ActivateAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, nullptr);
	}

	RemainingTime = 1.0f;
}


void UGA_SpellChargeProjectile::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           bool bReplicateEndAbility, bool bWasCancelled)
{

	MainPlayerCharacter->SetIsAiming(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
