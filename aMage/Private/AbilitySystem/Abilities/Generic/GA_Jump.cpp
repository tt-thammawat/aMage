// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Generic/GA_Jump.h"

#include "GameFramework/Character.h"

UGA_Jump::UGA_Jump()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

void UGA_Jump::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{


	// Activate the ability
	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		Super::InputPressed(Handle, ActorInfo, ActivationInfo);
		ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);

		if(Character->bIsCrouched)
		{
			Character->UnCrouch();
		}
		else
		{
			Character->Jump();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CommitAbility failed in UGA_Jump"));
		return;
	}
}

bool UGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(),ECastCheckedType::NullAllowed);

	return Character->CanJump() || Character->bIsCrouched;
}

void UGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if(!HasAuthorityOrPredictionKey(ActorInfo,&ActivationInfo))
	{
		return;
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	// Store these for later use in OnCharacterLand
	CurrentSpecHandle = Handle;
	CurrentActorInfo = ActorInfo;
	CurrentActivationInfo = ActivationInfo;
	
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	Character->LandedDelegate.AddDynamic(this,&ThisClass::OnCharacterLand);
}

void UGA_Jump::OnCharacterLand(const FHitResult& HitResult)
{
	if(HitResult.bBlockingHit)
	{
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UGA_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                             const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	Character->StopJumping();
}

void UGA_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	if(Character)
	{
		Character->LandedDelegate.RemoveDynamic(this,&ThisClass::OnCharacterLand);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

