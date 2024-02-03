// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/MainPlayerWidget.h"

void UMainPlayerWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	//Run in BLueprint
	WidgetControllerSet();
}

float UMainPlayerWidget::InterpolateGlobe(float CurrentPercent, float TargetPercent, float DeltaTime, float InterpSpeed)
{
	// Ensure InterpSpeed is not negative
	InterpSpeed = FMath::Max(InterpSpeed, 0.0f);

	// Calculate the new interpolated value
	return FMath::FInterpTo(CurrentPercent, TargetPercent, DeltaTime, InterpSpeed);
}
