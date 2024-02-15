// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/MainCastingGameplayAbility.h"

#include "UI/HUD/MainPlayerHUD.h"
#include "UI/Widget/MainPaintWidget.h"

void UMainCastingGameplayAbility::InputPressed(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if(PaintWidget == nullptr)
	{
		AMainPlayerHUD* MainPlayerHUD=  Cast<AMainPlayerHUD>(GetActorInfo().PlayerController->GetHUD());
		PaintWidget= MainPlayerHUD->DrawingWidget;
	}
	if(PaintWidget)
	{
		PaintWidget->SetVisibility(ESlateVisibility::Visible);
		if (APlayerController* PlayerController = GetActorInfo().PlayerController.Get())
		{
			if(ICastingInterface* CastingInterface = Cast<ICastingInterface>(PlayerController))
			{
				CastingInterface->SetIsCastingDrawingWidget(true);
			}
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			InputMode.SetWidgetToFocus(PaintWidget->TakeWidget());
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = true;
			PaintWidget->SetIsStartFocus(true);
		}
	}
	
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

}

void UMainCastingGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if(PaintWidget == nullptr)
	{
		AMainPlayerHUD* MainPlayerHUD=  Cast<AMainPlayerHUD>(GetActorInfo().PlayerController->GetHUD());
		PaintWidget= MainPlayerHUD->DrawingWidget;
	}
	
	if(PaintWidget)
	{
		PaintWidget->OnDrawingSpellSuccess.AddDynamic(this,&ThisClass::AddRuneTags);
	}
	
}

void UMainCastingGameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	
	if (APlayerController* PlayerController = GetActorInfo().PlayerController.Get())
	{
		if (ICastingInterface* CastingInterface = Cast<ICastingInterface>(PlayerController))
		{
			CastingInterface->SetIsCastingDrawingWidget(false);
		}
		
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
		
	
		if (PaintWidget)
		{
			PaintWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

}

void UMainCastingGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMainCastingGameplayAbility::AddRuneTags(FGameplayTag RuneTag)
{
	RuneTags.Add(RuneTag);
}
