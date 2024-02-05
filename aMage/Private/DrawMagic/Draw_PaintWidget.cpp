// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawMagic/Draw_PaintWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

int32 UDraw_PaintWidget::NativePaint(const FPaintArgs &Args, const FGeometry &AllottedGeometry, const FSlateRect &MyCullingRect, FSlateWindowElementList &OutDrawElements, int32 LayerId, const FWidgetStyle &InWidgetStyle, bool bParentEnabled) const
{
	Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	UWidgetBlueprintLibrary::DrawLines(Context, Points, FLinearColor::Blue, true, 10.0f);

	return LayerId + 1;
}

TArray<FVector2D> UDraw_PaintWidget::GetPoints()
{
	return Points;
}

void UDraw_PaintWidget::AddPoint(const FVector2D& Point)
{
	Points.Add(Point);
}

void UDraw_PaintWidget::RemoveAllPoints()
{
	Points.Empty();
}
