// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MainPlayerHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/MainPaintWidget.h"
#include "UI/Widget/MainPlayerWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void AMainPlayerHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
	UAttributeSet* AS)
{
	checkf(OverlayWidgetClass,TEXT("OverlayClass Uninitialized,MainHUD"))
	checkf(OverlayWidgetControllerClass,TEXT("OOverlayWidgetControllerClassd,MainHUD"))
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	OverlayWidget = Cast<UMainPlayerWidget>(Widget);
	UUserWidget* DrawWidget = CreateWidget<UUserWidget>(GetWorld(),DrawingWidgetClass);
	DrawingWidget = Cast<UMainPaintWidget>(DrawWidget);
	
	const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	
	OverlayWidget->SetWidgetController(WidgetController);
	DrawingWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValue();
	DrawingWidget->AddToViewport(1);
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


