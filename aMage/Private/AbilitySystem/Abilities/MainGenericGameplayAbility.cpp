// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/MainGenericGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/AbilityTask/InterpolateFOV.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Character/MainPlayerCharacter.h"
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
	if (bIsDebouncing) return;

	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	CurrentSpecHandle = Handle;
	CurrentActorInfo = ActorInfo;
	CurrentActivationInfo = ActivationInfo;
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_InputHeld, this, &ThisClass::ActivateAbilityAfterHeld, InputHeldDuration,bIsHeldLoop);
	
	bIsDebouncing = true;
	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, [this]()
		{ bIsDebouncing = false; }
		, 0.3f, false);
}

void UMainGenericGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	
	if(!MainPlayerCharacter)
	{
		AMainPlayerCharacter* Character = Cast<AMainPlayerCharacter>(CurrentActorInfo->AvatarActor.Get());
		if (Character)
		{
			MainPlayerCharacter = Character;
		}
	}
	
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

void UMainGenericGameplayAbility::ActivateAbilityAfterHeld()
{
	if (GetWorld()->GetTimeSeconds() >= InputHeldDuration)
	{
		ActivateAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, nullptr);
	}
}

void UMainGenericGameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                bool bReplicateCancelAbility)
{
	bIsCancel = true;
	ToggleCameraFOV(false);
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

void UMainGenericGameplayAbility::ToggleCameraFOV(bool IsActivate)
{
	if(IsActivate && InterpFOVTask && InterpFOVTask->IsActive()) return;
	
	if(!IsActivate && InterpFOVTask && InterpFOVTask->IsActive())
	{
		InterpFOVTask->EndTask();
	}
	
	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		if (UCameraComponent* Camera = Character->FindComponentByClass<UCameraComponent>())
		{
			float CurrentFOV = Camera->FieldOfView;
			float DesiredFOV = IsActivate ? NewFOV : DefaultFOV; 
			float InterpSpeed = IsActivate ? Duration:Duration/2;
			// Create and activate the interpolation task
			InterpFOVTask = UInterpolateFOV::InterpolateFOV(this, Camera, CurrentFOV, DesiredFOV, InterpSpeed);
			InterpFOVTask->OnInterpolationCompleted.AddDynamic(this,&ThisClass::ManualEndAbility);
			InterpFOVTask->ReadyForActivation();
		}
	}
}

void UMainGenericGameplayAbility::ManualEndAbility()
{
	if(bIsCancel)
	{
		EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,true);
	}
}

void UMainGenericGameplayAbility::RemoveAbilityAfterEnd(const TArray<TSubclassOf<UGameplayAbility>>& RemoveAbilities)
{
	UBaseAbilitySystemComponent* BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if(BaseAbilitySystemComponent)
	{
		BaseAbilitySystemComponent->RemoveCharacterAbilities(RemoveAbilities);
	}
}
