// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/MainPlayerWidget.h"
#include "MainPaintWidget.generated.h"

class AMainPlayerController;
/**
 * 
 */
UCLASS()
class AMAGE_API UMainPaintWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
private:
	void CheckDrawSpell();
	void StartDrawing();
	void SetUpMainPlayerController();
	UPROPERTY()
	TObjectPtr<AMainPlayerController> MainPlayerController;


//Drawing Canvas Line
	
protected:
	virtual int32 NativePaint(const FPaintArgs &Args, const FGeometry &AllottedGeometry, const FSlateRect &MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle &InWidgetStyle, bool bParentEnabled) const override;

public:
	TArray<FVector2D> GetPoints();
	void AddPoint(const FVector2D &Point);
	void RemoveAllPoints();

private:
	TArray<FVector2D> Points;
};
