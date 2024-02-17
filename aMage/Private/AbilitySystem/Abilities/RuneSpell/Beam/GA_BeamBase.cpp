// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/RuneSpell/Beam/GA_BeamBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/Beam/MainBeam.h"
#include "Character/MainPlayerCharacter.h"
#include "Interact/ICombatInterface.h"

UGA_BeamBase::UGA_BeamBase()
{
	NetExecutionPolicy=EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy=EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_BeamBase::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	//Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	bIsInputHeld = true;
	
	K2_ActivateAbility();

	// Set the timer to call the function repeatedly
	GetWorld()->GetTimerManager().SetTimer(AbilityActivationTimer, this, &UGA_BeamBase::RepeatedlyActivateAbility, 0.1f, true);
	
}

void UGA_BeamBase::RepeatedlyActivateAbility()
{
	if (bIsInputHeld)
	{
		// Reactivate or continue the ability based on your game logic
		K2_ActivateAbility();
	}
	else
	{
		// Input is no longer held, clear the timer
		GetWorld()->GetTimerManager().ClearTimer(AbilityActivationTimer);
		BeamREF->DeactivateBeam();
	}
}

void UGA_BeamBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMainPlayerCharacter* MainPlayerCharacter = CastChecked<AMainPlayerCharacter>(ActorInfo->AvatarActor.Get(),ECastCheckedType::NullAllowed);
	
	MainPlayerCharacter->SetIsAiming(true);
}

void UGA_BeamBase::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	bIsInputHeld = false;
	BeamREF->DeactivateBeam();
	GetWorld()->GetTimerManager().ClearTimer(AbilityActivationTimer);
}

void UGA_BeamBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AMainPlayerCharacter* MainPlayerCharacter = CastChecked<AMainPlayerCharacter>(ActorInfo->AvatarActor.Get(),ECastCheckedType::NullAllowed);

	MainPlayerCharacter->SetIsAiming(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FVector UGA_BeamBase::GetSocketLocation()
{
	//Get Avatar Actor That Cast This
	IICombatInterface* CombatInterface = Cast<IICombatInterface>(GetAvatarActorFromActorInfo());
	if(CombatInterface)
	{
		// Get SocketLocation FVector via ICombatInterface
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		return SocketLocation;
	}
	return FVector();
}

void UGA_BeamBase::SpawnBeam(const FVector& BeamEndLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer) return;
	//Get Avatar Actor That Cast This
	IICombatInterface* CombatInterface = Cast<IICombatInterface>(GetAvatarActorFromActorInfo());
	if(CombatInterface)
	{
		// Get SocketLocation FVector via ICombatInterface
		FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (BeamEndLocation - SocketLocation).Rotation();
		FRotator SpawnRotation = Rotation;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(SpawnRotation.Quaternion());

		BeamREF = GetWorld()->SpawnActorDeferred<AMainBeam>(
							BeamClass,
							SpawnTransform,
							GetOwningActorFromActorInfo(),
							Cast<APawn>(GetOwningActorFromActorInfo()),
							ESpawnActorCollisionHandlingMethod::AlwaysSpawn
							);

		BeamREF->SetOwner(GetOwningActorFromActorInfo());
		
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		//Make Spec Handle That Contains Effect Information And Send It To Projectile
		const FGameplayEffectSpecHandle SpecHandle= SourceASC->MakeOutgoingSpec(DamageEffectClass,GetAbilityLevel(),SourceASC->MakeEffectContext());
		//Now The Projectile Have The Damage From This Spell
		BeamREF->DamageEffectSpecHandle=SpecHandle;
		BeamREF->FinishSpawning(SpawnTransform);
		
	}
}
