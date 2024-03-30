// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/RuneSpell/Buff/GA_HoldBuff.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsSingleton.h"
#include "Character/MainPlayerCharacter.h"

void UGA_HoldBuff::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(FMainGameplayTags::Get().State_Movement_Crouch))
	{
		return;
	}
	
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	bIsInputHeld = true;
}

void UGA_HoldBuff::ActivateAbilityAfterHeld()
{
	Super::ActivateAbilityAfterHeld();
	if (bIsInputHeld)
	{
		ActivateAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, nullptr);
	}
	else
	{
		RemoveActiveEffects();
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_InputHeld);
	}
}

void UGA_HoldBuff::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	MainPlayerCharacter->SetIsAiming(true);
}

void UGA_HoldBuff::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	
	bIsInputHeld = false;
	
	RemoveActiveEffects();
	
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_InputHeld);

}

void UGA_HoldBuff::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	MainPlayerCharacter->SetIsAiming(false);
	
	RemoveActiveEffects();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_HoldBuff::RemoveActiveEffects()
{
	if(HasAuthority(&CurrentActivationInfo))
	{
		if(!ActiveHandle.IsValid()) return;
		
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(ActiveHandle,INT32_MAX);
		ActiveHandle.Invalidate();
	}
	else
	{
		ServerRemoveActiveEffect();
	}
}

void UGA_HoldBuff::ServerRemoveActiveEffect_Implementation()
{
	if(!ActiveHandle.IsValid()) return;
		
	GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(ActiveHandle,INT32_MAX);
	ActiveHandle.Invalidate();
}
