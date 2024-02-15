// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/MainCastingGameplayAbility.h"

#include "UI/HUD/MainPlayerHUD.h"
#include "UI/Widget/MainPaintWidget.h"

void UMainCastingGameplayAbility::InputPressed(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

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
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

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
			PaintWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);


}

void UMainCastingGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	// Iterate over RuneTags and print each one
	for (const FGameplayTag& Tag : RuneTags)
	{
		FString TagString = Tag.ToString();
		UE_LOG(LogTemp, Warning, TEXT("Rune Tag: %s"), *TagString);
	}
	//TODO: Use CastingInterface To Send Those Tag To PlayerCharacter
	RuneTags.Empty();
	
	if(PaintWidget)
	{
		PaintWidget->OnDrawingSpellSuccess.RemoveDynamic(this, &ThisClass::AddRuneTags);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}

void UMainCastingGameplayAbility::AddRuneTags(FGameplayTag RuneTag)
{
	RuneTags.Add(RuneTag);
}
