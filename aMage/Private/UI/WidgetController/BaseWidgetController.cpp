// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/BaseWidgetController.h"

void UBaseWidgetController::SetWidgetControllerParam(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UBaseWidgetController::BroadcastInitialValue()
{
}

void UBaseWidgetController::BindCallbacksToDependencies()
{
}
