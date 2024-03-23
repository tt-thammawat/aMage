// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Enemies/GA_EnemyProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsSingleton.h"
#include "Actor/Projectile/MainProjectile.h"

void UGA_EnemyProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

FVector UGA_EnemyProjectileSpell::GetSocketLocation()
{
	//Get Avatar Actor That Cast This
	IICombatInterface* CombatInterface = Cast<IICombatInterface>(GetAvatarActorFromActorInfo());
	if(CombatInterface)
	{
		// Get SocketLocation FVector via ICombatInterface
		const FVector SocketLocation = 	IICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),FMainGameplayTags::Get().Montage_Attack_Weapon);
		return SocketLocation;
	}
	return FVector();
}

void UGA_EnemyProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation,
	const FVector& CharSocketLocation, int32 NumProjectiles)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer) return;
	
		// Get SocketLocation FVector via ICombatInterface
		const FVector SocketLocation = 	CharSocketLocation;
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
					GetAvatarActorFromActorInfo(),
					Cast<APawn>(GetAvatarActorFromActorInfo()),
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

			for (auto& Pair : DamageTypes)
			{
				const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,Pair.Key,ScaledDamage);
			}
		
			//Now The Projectile Have The Damage From This Spell
			Projectile->DamageEffectSpecHandle=SpecHandle;
			Projectile->FinishSpawning(SpawnTransform);
		}
}
