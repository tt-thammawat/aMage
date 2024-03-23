// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/RuneSpell/Beam/GA_BeamBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsSingleton.h"
#include "Actor/Beam/MainBeam.h"
#include "Character/MainPlayerCharacter.h"
#include "Interact/ICombatInterface.h"
#include "Net/UnrealNetwork.h"

UGA_BeamBase::UGA_BeamBase()
{
	NetExecutionPolicy=EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy=EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_BeamBase::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(FMainGameplayTags::Get().State_Movement_Crouch))
	{
		return;
	}
	
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	bIsInputHeld = true;
	ActivateAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, nullptr);
	
	
}

void UGA_BeamBase::ActivateAbilityAfterHeld()
{
	if (bIsInputHeld)
	{
		// Reactivate or continue the ability based on your game logic
		ActivateAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, nullptr);
	}
	else
	{
		// Input is no longer held, clear the timer
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_InputHeld);
		BeamREF->DeactivateBeam();
	}
}

void UGA_BeamBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	MainPlayerCharacter->SetIsAiming(true);
	

}

FVector UGA_BeamBase::GetSocketLocation()
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

void UGA_BeamBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UGA_BeamBase,BeamREF,COND_OwnerOnly);
}

void UGA_BeamBase::SpawnBeam(const FVector& BeamEndLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(bIsServer)
	{
		// Get SocketLocation FVector via ICombatInterface
		const FVector SocketLocation = 	IICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),FMainGameplayTags::Get().Montage_Attack_Weapon);
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

		//Give The Projectile A Gameplay Effect Spec For Causing Damage
		//Get AbilitySystemComponent
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		//Set AbilityCDO //Add Those Things to check if information is pass along
		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(BeamREF);
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(BeamREF);
		EffectContextHandle.AddActors(Actors);
		FHitResult HitResult;
		HitResult.Location = BeamEndLocation;
		EffectContextHandle.AddHitResult(HitResult);
		
		//Make Spec Handle That Contains Effect Information And Send It To Projectile
		const FGameplayEffectSpecHandle SpecHandle= SourceASC->MakeOutgoingSpec(EffectClass,GetAbilityLevel(),EffectContextHandle);
		const FMainGameplayTags MainGameplayTags = FMainGameplayTags::Get();

		for (auto& Pair : DamageTypes)
		{
			const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,Pair.Key,ScaledDamage);
		}
		
		BeamREF->DamageEffectSpecHandle=SpecHandle;
		BeamREF->FinishSpawning(SpawnTransform);
	}
}

void UGA_BeamBase::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	bIsInputHeld = false;
	if(BeamREF)
	{
		BeamREF->DeactivateBeam();
	}
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_InputHeld);
}

void UGA_BeamBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	MainPlayerCharacter->SetIsAiming(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

