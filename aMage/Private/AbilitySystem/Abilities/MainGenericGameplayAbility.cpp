// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/MainGenericGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

void UMainGenericGameplayAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UMainGenericGameplayAbility,UsageTimes,COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UMainGenericGameplayAbility,bIsCancel,COND_OwnerOnly);

}

void UMainGenericGameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	bIsCancel = true;
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

bool UMainGenericGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;

	if(bIsCancel == true)
	{
		return false;
	}
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}


void UMainGenericGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass,1);
	for(TTuple<FGameplayTag, FScalableFloat>& Pair : DamageType)
	{
		if(!Pair.Value.Curve.IsNull())
		{
			float DamageMagnitude = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,Pair.Key,DamageMagnitude);
		}
		
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}


void UMainGenericGameplayAbility::RemoveAbilityAfterEnd(const TArray<TSubclassOf<UGameplayAbility>>& RemoveAbilities)
{
	UBaseAbilitySystemComponent* BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if(BaseAbilitySystemComponent)
	{
			BaseAbilitySystemComponent->RemoveCharacterAbilities(RemoveAbilities);
	}
}