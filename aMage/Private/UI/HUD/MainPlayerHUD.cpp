// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MainPlayerHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/MainPlayerWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void AMainPlayerHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
	UAttributeSet* AS)
{
	checkf(OverlayWidgetClass,TEXT("OverlayClass Uninitialized,MainHUD"))
	checkf(OverlayWidgetControllerClass,TEXT("OOverlayWidgetControllerClassd,MainHUD"))
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	OverlayWidget = Cast<UMainPlayerWidget>(Widget);
	
	UUserWidget* ESCWidgetRef = CreateWidget<UUserWidget>(GetWorld(),ESCWidgetClass);
	ESCWidget  = Cast<UMainPlayerWidget>(ESCWidgetRef);
	
	const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	
	ESCWidget->SetWidgetController(WidgetController);
	
	OverlayWidget->SetWidgetController(WidgetController);
	
	WidgetController->BroadcastInitialValue();
	ESCWidgetRef->AddToViewport(24);
	
	Widget->AddToViewport(0);

}

UOverlayWidgetController* AMainPlayerHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController == nullptr)
	{
		//Create New WidgetController Object
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParam(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
		return OverlayWidgetController;
		
	}
	return OverlayWidgetController;
}


