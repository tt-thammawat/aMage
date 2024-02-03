// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/MainProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/Projectile/MainProjectile.h"
#include "Interact/ICombatInterface.h"

void UMainProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UMainProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
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
		//Set Projectile Fly Parallel to the ground
		Rotation.Pitch = 0.f;
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
		
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
		//Make Spec Handle That Contains Effect Information And Send It To Projectile
		const FGameplayEffectSpecHandle SpecHandle= SourceASC->MakeOutgoingSpec(DamageEffectClass,GetAbilityLevel(),SourceASC->MakeEffectContext());
		//Now The Projectile Have The Damage From This Spell
		Projectile->DamageEffectSpecHandle=SpecHandle;
		Projectile->FinishSpawning(SpawnTransform);
	}

}
