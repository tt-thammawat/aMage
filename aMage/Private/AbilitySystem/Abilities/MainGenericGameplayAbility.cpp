// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/MainGenericGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/MainPaintWidget.h"
#include "Net/UnrealNetwork.h"

void UMainGenericGameplayAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UMainGenericGameplayAbility,UsageTimes,COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UMainGenericGameplayAbility,bIsCancel,COND_OwnerOnly);

}

void UMainGenericGameplayAbility::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	CurrentSpecHandle = Handle;
	CurrentActorInfo = ActorInfo;
	CurrentActivationInfo = ActivationInfo;
}

void UMainGenericGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	APlayerController* PlayerController = GetActorInfo().PlayerController.Get();
	if (PlayerController && PlayerController->IsLocalPlayerController())
	{
		if(!SpellIndicator)
		{
			SpellIndicator = CreateWidget<UMainPlayerWidget>(GetActorInfo().PlayerController->GetWorld(),SpellIndicatorClass);
			if (SpellIndicator)
			{
				SpellIndicator->SetWidgetController(this);
				SpellIndicator->AddToViewport(2);
			}
		}

	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
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

void UMainGenericGameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle,
												const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
												bool bReplicateCancelAbility)
{
	bIsCancel = true;
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMainGenericGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if(SpellIndicator)
	{
		SpellIndicator->RemoveFromParent();
		SpellIndicator = nullptr;
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMainGenericGameplayAbility::RemoveAbilityAfterEnd(const TArray<TSubclassOf<UGameplayAbility>>& RemoveAbilities)
{
	UBaseAbilitySystemComponent* BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if(BaseAbilitySystemComponent)
	{
		BaseAbilitySystemComponent->RemoveCharacterAbilities(RemoveAbilities);
	}
}