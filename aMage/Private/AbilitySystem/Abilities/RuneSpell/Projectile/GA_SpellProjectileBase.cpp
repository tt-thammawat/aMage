// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/RuneSpell/Projectile/GA_SpellProjectileBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsSingleton.h"
#include "Actor/Projectile/MainProjectile.h"
#include "Character/MainPlayerCharacter.h"
#include "Interact/ICombatInterface.h"


UGA_SpellProjectileBase::UGA_SpellProjectileBase() 
{
	NetExecutionPolicy=EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	InstancingPolicy=EGameplayAbilityInstancingPolicy::InstancedPerActor;
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
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_InputHeld, this, &ThisClass::ActivateAbilityAfterHold, InputHeldDuration);
}

void UGA_SpellProjectileBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	AMainPlayerCharacter* MainPlayerCharacter = CastChecked<AMainPlayerCharacter>(ActorInfo->AvatarActor.Get(),ECastCheckedType::NullAllowed);
	MainPlayerCharacter->SetIsAiming(true);
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_SpellProjectileBase::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_InputHeld))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_InputHeld);

	}
}

void UGA_SpellProjectileBase::ActivateAbilityAfterHold()
{
	if (GetWorld()->GetTimeSeconds() - InputPressTime >= InputHeldDuration)
	{
		ActivateAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, nullptr);
	}
}

FVector UGA_SpellProjectileBase::GetSocketLocation()
{
	//Get Avatar Actor That Cast This
	IICombatInterface* CombatInterface = Cast<IICombatInterface>(GetAvatarActorFromActorInfo());
	if(CombatInterface)
	{
		// Get SocketLocation FVector via ICombatInterface
		const FVector SocketLocation = 	IICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo());
		return SocketLocation;
	}
	return FVector();
}

void UGA_SpellProjectileBase::SpawnProjectile(const FVector& ProjectileTargetLocation,const FVector& CharSocketLocation, int32 NumProjectiles)
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


void UGA_SpellProjectileBase::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	AMainPlayerCharacter* MainPlayerCharacter = CastChecked<AMainPlayerCharacter>(ActorInfo->AvatarActor.Get(),ECastCheckedType::NullAllowed);

	MainPlayerCharacter->SetIsAiming(false);
	
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_SpellProjectileBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	AMainPlayerCharacter* MainPlayerCharacter = CastChecked<AMainPlayerCharacter>(ActorInfo->AvatarActor.Get(),ECastCheckedType::NullAllowed);

	MainPlayerCharacter->SetIsAiming(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
