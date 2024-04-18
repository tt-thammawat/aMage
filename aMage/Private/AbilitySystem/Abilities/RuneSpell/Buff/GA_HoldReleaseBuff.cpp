// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/RuneSpell/Buff/GA_HoldReleaseBuff.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagsSingleton.h"
#include "Character/MainPlayerCharacter.h"


UGA_HoldReleaseBuff::UGA_HoldReleaseBuff()
{
	NetExecutionPolicy=EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	InstancingPolicy=EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGA_HoldReleaseBuff::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_HoldReleaseBuff::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	
	if (ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(FMainGameplayTags::Get().State_Movement_Crouch))
	{
		return;
	}
	
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	ToggleCameraFOV(true);
	bIsPressed=true;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_UpdateHeldValue, this, &UGA_HoldReleaseBuff::UpdateValueDuringHold, 0.05f,true);
}

void UGA_HoldReleaseBuff::ActivateAbilityAfterHeld()
{
	// Used To Activated Ability Here But Now Move It To InputRelease
}

void UGA_HoldReleaseBuff::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	MainPlayerCharacter->SetIsAiming(true);
}

void UGA_HoldReleaseBuff::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_InputHeld))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_InputHeld);
	}

	if (RemainingTime <= 0.1f)
	{
		ActivateAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, nullptr);
	}
	bIsPressed = false;
	ToggleCameraFOV(false);
	RemainingTime = 1.0f;
}

void UGA_HoldReleaseBuff::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	MainPlayerCharacter->SetIsAiming(false);
	bIsPressed = false;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_HoldReleaseBuff::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	bIsPressed = false;

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_HoldReleaseBuff::UpdateValueDuringHold()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_InputHeld))
	{
		RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_InputHeld);
		GetValueWhenPressed(RemainingTime,bIsPressed);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_UpdateHeldValue);
		GetValueWhenPressed(0.f,bIsPressed);
	}
}