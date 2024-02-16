// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Generic/GA_Crouch.h"

#include "GameFramework/Character.h"

UGA_Crouch::UGA_Crouch()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

void UGA_Crouch::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	if(CommitAbility(Handle,ActorInfo,ActivationInfo))
	{
		if(ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(),ECastCheckedType::NullAllowed))
		{
			if(Character->bIsCrouched)
			{
				Character->UnCrouch();
				// End the ability immediately after uncrouching
				EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			}
			else
			{
				Character->Crouch();
			}
			
		}
		Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CommitAbility failed in UGA_Crouch"));
		return;
	}
}

bool UGA_Crouch::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	
	const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(),ECastCheckedType::NullAllowed);
	return Character->CanCrouch();
}

void UGA_Crouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UGA_Crouch::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}