// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/MainGenericGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

void UMainGenericGameplayAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UMainGenericGameplayAbility,UsageTimes,COND_OwnerOnly);
}

void UMainGenericGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
	{
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();

		//Case Not Remove Effect
		for(auto& GameplayEffect : OngoingEffectsToApplyOnstart)
		{
			if(!GameplayEffect.Get()) continue;

			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect,1,EffectContextHandle);

			if(SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle GameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				if(!GameplayEffectHandle.WasSuccessfullyApplied())
				{
					UE_LOG(LogTemp, Warning, TEXT("Ability %s Failed to apply Effect %s"),*GetName(), *GetNameSafe(GameplayEffect));
				}
			}
		}
		if(IsInstantiated())
		{
			for(auto& GameplayEffect : OngoingEffectsToRemoveOnEnd)
			{
				//not valid skip this one and proceed to check the next gameplay effect in the loop.
				if(!GameplayEffect.Get()) continue;

				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect,1,EffectContextHandle);
				if(SpecHandle.IsValid())
				{
					FActiveGameplayEffectHandle GameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		
					if(!GameplayEffectHandle.WasSuccessfullyApplied())
					{
						UE_LOG(LogTemp, Warning, TEXT("Ability %s Failed to apply Effect %s"),*GetName(), *GetNameSafe(GameplayEffect));

					}
					else
					{
						//Add Handle To Remove This Effect Just In Case The Effect Can be removed
						RemoveOnEndEffectHandle.Add(GameplayEffectHandle);
					}
				}
			}
		}
	}
}

void UMainGenericGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	//Instanced Abilities: unique instances of an ability for each actor. Each actor gets its own copy of the ability it own state ,own information
	if(IsInstantiated())
	{
		if(UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
		{
			for (FActiveGameplayEffectHandle& ActiveGameplayEffectHandle : RemoveOnEndEffectHandle)
			{
				if(ActiveGameplayEffectHandle.IsValid())
				{
					AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveGameplayEffectHandle);
				}
			}
		}
		//clear handle when abilities end
		RemoveOnEndEffectHandle.Empty();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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
