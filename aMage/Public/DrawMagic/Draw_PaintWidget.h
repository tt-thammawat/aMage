// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/Vector2D.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Draw_PaintWidget.generated.h"

/**
 * 
 */
UCLASS()
class AMAGE_API UDraw_PaintWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual int32 NativePaint(const FPaintArgs &Args, const FGeometry &AllottedGeometry, const FSlateRect &MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle &InWidgetStyle, bool bParentEnabled) const override;

public:
	TArray<FVector2D> GetPoints();
	void AddPoint(const FVector2D &Point);
	void RemoveAllPoints();

private:
	TArray<FVector2D> Points;
	
};
