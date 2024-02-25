// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/RuneSpell/Projectile/GA_SpellChargeProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsSingleton.h"
#include "Actor/Projectile/ChargeProjectile.h"
#include "Character/MainPlayerCharacter.h"
#include "Net/UnrealNetwork.h"

UGA_SpellChargeProjectile::UGA_SpellChargeProjectile():
bIsPlayingAnimation(false)
{
	NetExecutionPolicy=EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy=EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_SpellChargeProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(UGA_SpellChargeProjectile,bIsPlayingAnimation,COND_OwnerOnly);

}

void UGA_SpellChargeProjectile::InputPressed(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	//Remove These Super Because it make ActivateAbility Run Twice
	//Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	InputPressTime = GetWorld()->GetTimeSeconds();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_InputHeld, this, &UGA_SpellChargeProjectile::ActivateAbilityAfterHold, InputHeldDuration);

}

void UGA_SpellChargeProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	//Remove These Super Because it make ActivateAbility Run Twice
	//Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMainPlayerCharacter* MainPlayerCharacter = CastChecked<AMainPlayerCharacter>(ActorInfo->AvatarActor.Get(),ECastCheckedType::NullAllowed);
	
	MainPlayerCharacter->SetIsAiming(true);
}

void UGA_SpellChargeProjectile::SpawnChargeProjectile(const FVector& ProjectileTargetLocation, int32 NumProjectiles)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer) return;
		// Get SocketLocation FVector via ICombatInterface
		const FVector SocketLocation = 	IICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo());		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		//Set Projectile Fly Parallel to the ground
		Rotation.Pitch = 0.f;

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

void UGA_SpellChargeProjectile::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           bool bReplicateEndAbility, bool bWasCancelled)
{
	AMainPlayerCharacter* MainPlayerCharacter = CastChecked<AMainPlayerCharacter>(ActorInfo->AvatarActor.Get(),ECastCheckedType::NullAllowed);

	MainPlayerCharacter->SetIsAiming(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_SpellChargeProjectile::ActivateAbilityAfterHold()
{
	if (GetWorld()->GetTimeSeconds() - InputPressTime >= InputHeldDuration)
	{
		K2_ActivateAbility();
	}
}

void UGA_SpellChargeProjectile::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	//Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_InputHeld))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_InputHeld);

	}
}
