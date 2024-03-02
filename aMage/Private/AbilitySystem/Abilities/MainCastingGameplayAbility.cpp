// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/MainCastingGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilitySystem/AbilityTask/InterpolateFOV.h"
#include "Camera/CameraComponent.h"
#include "Character/MainPlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interact/CastingInterface.h"
#include "Net/UnrealNetwork.h"
#include "UI/HUD/MainPlayerHUD.h"
#include "UI/Widget/MainPaintWidget.h"

UMainCastingGameplayAbility::UMainCastingGameplayAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMainCastingGameplayAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMainCastingGameplayAbility,bIsCancel);
}

void UMainCastingGameplayAbility::OnRep_bIsCancel()
{
	bIsAbilityActive=false;
	ToggleDrawingMode(false);
	DeactivateDrawingMode();
}

void UMainCastingGameplayAbility::InputPressed(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (bIsDebouncing) return;
	// Toggle the ability activation state.
	bIsAbilityActive = !bIsAbilityActive;
	
		if(bIsAbilityActive)
		{
			// The ability has just been activated.
			ActivateDrawingMode();
		}
		else
		{
			// The ability is being deactivated.
			DeactivateDrawingMode();
		}

	bIsDebouncing = true;
	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, [this]()
		{ bIsDebouncing = false; }
		, 0.3f, false);
}

void UMainCastingGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (GetActorInfo().PlayerController->IsLocalPlayerController())
	{
		if(PaintWidget == nullptr)
		{
			const AMainPlayerHUD* MainPlayerHUD=  Cast<AMainPlayerHUD>(GetActorInfo().PlayerController->GetHUD());
			PaintWidget= MainPlayerHUD->DrawingWidget;
		}
	
		if(PaintWidget)
		{
			PaintWidget->OnDrawingSpellSuccess.AddDynamic(this,&ThisClass::AddRuneTags);
			PaintWidget->OnDrawingClearSpellSuccessSignature.AddDynamic(this,&ThisClass::ClearRuneTags);
		}
	}
	CurrentSpecHandle=Handle;
	CurrentActorInfo=ActorInfo;
	CurrentActivationInfo=ActivationInfo;
	
	//TODO : Move This WalkSpeed To Attribute Fix This Cause Client Doesn't Work
	//Set Character WalkSpeed
	const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(),ECastCheckedType::NullAllowed);
	OldMaxWalkSpeed=Character->GetCharacterMovement()->MaxWalkSpeed;
	Character->GetCharacterMovement()->MaxWalkSpeed = SlowMaxWalkSpeed;
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	


}

void UMainCastingGameplayAbility::ToggleDrawingMode(bool IsActivate)
{
	if(InterpFOVTask && InterpFOVTask->IsActive())
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

void UMainCastingGameplayAbility::ClientCancelAbilities_Implementation()
{
	bIsAbilityActive=false;
	ToggleDrawingMode(false);
	DeactivateDrawingMode();
}

void UMainCastingGameplayAbility::ManualEndAbility()
{
	if(bIsAbilityActive) return;
	EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,false);
}

void UMainCastingGameplayAbility::ActivateDrawingMode()
{
	if (GetActorInfo().PlayerController->IsLocalPlayerController())
	{
		if(PaintWidget == nullptr)
		{
			const AMainPlayerHUD* MainPlayerHUD=  Cast<AMainPlayerHUD>(GetActorInfo().PlayerController->GetHUD());
			PaintWidget= MainPlayerHUD->DrawingWidget;
		}
		if(PaintWidget)
		{
			PaintWidget->SetVisibility(ESlateVisibility::Visible);
			if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetActorInfo().PlayerController.Get()))
			{
				PlayerController->SetbIsDrawingSpell(true);
				FInputModeGameAndUI InputMode;
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
				InputMode.SetWidgetToFocus(PaintWidget->TakeWidget());
				PlayerController->SetInputMode(InputMode);
				PlayerController->bShowMouseCursor = true;
				PaintWidget->SetIsStartFocus(true);

				//Set Mouse To Center
				int32 ViewportSizeX, ViewportSizeY;
				PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
				int32 CenterX = ViewportSizeX / 2;
				int32 CenterY = ViewportSizeY / 2;

				PlayerController->SetMouseLocation(CenterX, CenterY);
			}
		}
		//Camera
		if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
		{
			if (UCameraComponent* Camera = Character->FindComponentByClass<UCameraComponent>())
			{
				if (Camera)
				{
					ToggleDrawingMode(true);
					
					DefaultVignetteIntensity = Camera->PostProcessSettings.VignetteIntensity;
					Camera->PostProcessSettings.VignetteIntensity = NewVignetteIntensity;
					
					DefaultDepthOfFieldVignetteSize= Camera->PostProcessSettings.DepthOfFieldVignetteSize;
					Camera->PostProcessSettings.DepthOfFieldVignetteSize=NewDepthOfFieldVignetteSize;
				}
			}
		}
	}
}

void UMainCastingGameplayAbility::AddRuneTags()
{
	ACharacter* Character = CastChecked<ACharacter>(GetActorInfo().AvatarActor.Get(),ECastCheckedType::NullAllowed);
	ICastingInterface* CastingInterface = CastChecked<ICastingInterface>(Character);
	if(CastingInterface)
	{
		CastingInterface->MatchRuneSpellTags(PaintWidget->GetRuneTags());
	}
}

void UMainCastingGameplayAbility::ClearRuneTags()
{
	ACharacter* Character = CastChecked<ACharacter>(GetActorInfo().AvatarActor.Get(),ECastCheckedType::NullAllowed);
	ICastingInterface::Execute_ClearRuneSpell(Character);
}

void UMainCastingGameplayAbility::DeactivateDrawingMode()
{
	if (GetActorInfo().PlayerController->IsLocalPlayerController())
	{
		if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetActorInfo().PlayerController.Get()))
		{
			const FInputModeGameOnly InputMode;
			PlayerController->SetbIsDrawingSpell(false);
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = false;
			
			if (PaintWidget)
			{
				PaintWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		
		if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
		{
			if (UCameraComponent* Camera = Character->FindComponentByClass<UCameraComponent>())
			{
				ToggleDrawingMode(false);
				
				Camera->PostProcessSettings.VignetteIntensity = DefaultVignetteIntensity;
				Camera->PostProcessSettings.DepthOfFieldVignetteSize=DefaultDepthOfFieldVignetteSize;
			}
		}
	}
}

void UMainCastingGameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                bool bReplicateCancelAbility)
{
	//server make the client disable visual
	ClientCancelAbilities();
}


void UMainCastingGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             bool bReplicateEndAbility, bool bWasCancelled)
{
	
	if(InterpFOVTask)
	{
		InterpFOVTask->OnInterpolationCompleted.RemoveDynamic(this,&ThisClass::ManualEndAbility);
	}
	
	if(PaintWidget)
	{
		PaintWidget->OnDrawingSpellSuccess.RemoveDynamic(this, &ThisClass::AddRuneTags);
		PaintWidget->OnDrawingClearSpellSuccessSignature.RemoveDynamic(this,&ThisClass::ClearRuneTags);
	}
	
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(),ECastCheckedType::NullAllowed);

	//Set Character WalkSpeed
	Character->GetCharacterMovement()->MaxWalkSpeed= OldMaxWalkSpeed;
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
