// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/Widget/MainPlayerWidget.h"
#include "MainPaintWidget.generated.h"

class AMainPlayerController;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDrawingSpellSuccessSignature,FGameplayTag,RuneNameTag);
UCLASS()
class AMAGE_API UMainPaintWidget : public UMainPlayerWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable,Category="PlayerController")
	void SetUpMainPlayerController(APlayerController* PlayerController);
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UPROPERTY(BlueprintAssignable)
	FOnDrawingSpellSuccessSignature OnDrawingSpellSuccess;
	UFUNCTION(BlueprintCallable)
	void SetIsStartFocus(bool bStartFocus) { bIsStartFocus = bStartFocus;};
private:
	
	void CheckDrawSpell();
	bool bIsStartFocus=false;
	bool bIsDrawing=false;
	UPROPERTY()
	TObjectPtr<AMainPlayerController> MainPlayerController;


//Drawing Canvas Line
	
protected:
	virtual int32 NativePaint(const FPaintArgs &Args, const FGeometry &AllottedGeometry, const FSlateRect &MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle &InWidgetStyle, bool bParentEnabled) const override;

public:
	void AddPoint(const FVector2D &Point);
	UFUNCTION(BlueprintCallable,Category=Default)	
	void RemoveAllPoints();

private:
	//Line Visual
	TArray<TArray<FVector2D>> LineSegments;
	TArray<FVector2D> Points;
};
