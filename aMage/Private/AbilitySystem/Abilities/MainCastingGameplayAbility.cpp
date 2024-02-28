// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/MainCastingGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Camera/CameraComponent.h"
#include "Character/MainPlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interact/CastingInterface.h"
#include "UI/HUD/MainPlayerHUD.h"
#include "UI/Widget/MainPaintWidget.h"

UMainCastingGameplayAbility::UMainCastingGameplayAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
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
		, 0.2f, false);

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

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	//TODO : Move This WalkSpeed To Attribute Fix This Cause Client Doesn't Work
	//Set Character WalkSpeed
	const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(),ECastCheckedType::NullAllowed);
	OldMaxWalkSpeed=Character->GetCharacterMovement()->MaxWalkSpeed;
	Character->GetCharacterMovement()->MaxWalkSpeed = SlowMaxWalkSpeed;

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
				DefaultFOV = Camera->FieldOfView;
				Camera->SetFieldOfView(NewFOV); 
				DefaultVignetteIntensity = Camera->PostProcessSettings.VignetteIntensity;
				Camera->PostProcessSettings.VignetteIntensity = NewVignetteIntensity;
				DefaultDepthOfFieldVignetteSize= Camera->PostProcessSettings.DepthOfFieldVignetteSize;
				Camera->PostProcessSettings.DepthOfFieldVignetteSize=NewDepthOfFieldVignetteSize;
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
	ICastingInterface* CastingInterface = CastChecked<ICastingInterface>(Character);
	if(CastingInterface)
	{
	  CastingInterface->ClearRuneSpell();
	}
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
				Camera->SetFieldOfView(DefaultFOV); 
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
	DeactivateDrawingMode();
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMainCastingGameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if(!bIsAbilityActive)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo,true,false);
		Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	}
}

void UMainCastingGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             bool bReplicateEndAbility, bool bWasCancelled)
{
	
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
