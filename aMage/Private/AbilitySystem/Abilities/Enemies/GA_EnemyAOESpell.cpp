// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Enemies/GA_EnemyAOESpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsSingleton.h"
#include "Actor/Projectile/ChargeProjectile.h"

void UGA_EnemyAOESpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

FVector UGA_EnemyAOESpell::GetSocketLocation()
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

void UGA_EnemyAOESpell::SpawnAOE(AActor* PlayerActor)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer) return;

	FVector PlayerLocation = PlayerActor->GetActorLocation();
	FVector GroundLocation;

	// Trace parameters
	FHitResult OutHit;
	FVector Start = PlayerLocation + FVector(0.f, 0.f, 50.f); // Start slightly above the player
	FVector End = PlayerLocation - FVector(0.f, 0.f, 1000.f); // Extend the trace well below the player
	FCollisionQueryParams CollisionParams;

	// Perform the trace
	bool bFoundGround = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

	if (bFoundGround)
	{
		GroundLocation = OutHit.ImpactPoint; // Use the impact point as the ground location
	}
	else
	{
		GroundLocation = PlayerLocation; // Fallback to player's location if ground not found
	}
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GroundLocation);
	SpawnTransform.SetRotation(FRotator::ZeroRotator.Quaternion());

	AOERef = GetWorld()->SpawnActorDeferred<AChargeProjectile>(
		AOEClass,
		SpawnTransform,
		GetAvatarActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	//Give The Projectile A Gameplay Effect Spec For Causing Damage
	//Get AbilitySystemComponent
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	//Set AbilityCDO //Add Those Things to check if information is pass along
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(AOERef);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(AOERef);
	EffectContextHandle.AddActors(Actors);
	FHitResult HitResult;
	HitResult.Location = GroundLocation;
	EffectContextHandle.AddHitResult(HitResult);
	
	//Make Spec Handle That Contains Effect Information And Send It To Projectile
	const FGameplayEffectSpecHandle DamageSpecHandle= SourceASC->MakeOutgoingSpec(DamageEffectClass,GetAbilityLevel(),EffectContextHandle);
	const FGameplayEffectSpecHandle EffectSpecHandle= SourceASC->MakeOutgoingSpec(EffectClass,GetAbilityLevel(),EffectContextHandle);
	const FMainGameplayTags MainGameplayTags = FMainGameplayTags::Get();

	for (auto& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,Pair.Key,ScaledDamage);
	}
	
	if (AOERef)
	{
		AOERef->SetTargetActor(PlayerActor);
		AOERef->DamageEffectSpecHandle=DamageSpecHandle;
		AOERef->EffectSpecHandle=EffectSpecHandle;
		AOERef->FinishSpawning(SpawnTransform);
	}
}
