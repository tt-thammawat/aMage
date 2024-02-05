// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MainPlayerHUD.h"

#include "Blueprint/UserWidget.h"
#include "DrawMagic/Draw_PaintWidget.h"
#include "UI/Widget/MainPlayerWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void AMainPlayerHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
	UAttributeSet* AS)
{
	checkf(OverlayWidgetClass,TEXT("OverlayClass Uninitialized,MainHUD"))
	checkf(OverlayWidgetControllerClass,TEXT("OOverlayWidgetControllerClassd,MainHUD"))
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	OverlayWidget = Cast<UMainPlayerWidget>(Widget);
	
	const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValue();
	Widget->AddToViewport();

}

void AMainPlayerHUD::InitDrawingWidget()
{
	PaintWidget = CreateWidget<UDraw_PaintWidget>(GetWorld(), PaintWidgetClass);
	if (PaintWidget != nullptr)
	{
		PaintWidget->AddToViewport();
		PaintWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
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
