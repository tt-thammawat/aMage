// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/Widget/MainPlayerWidget.h"
#include "MainPaintWidget.generated.h"


USTRUCT(BlueprintType)
struct FAbilitiesTagList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameplayTag> Tags;
};

class AMainPlayerController;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDrawingSpellSuccessSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDrawingClearSpellSuccessSignature);

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
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

	UPROPERTY(BlueprintAssignable)
	FOnDrawingSpellSuccessSignature OnDrawingSpellSuccess;
	UPROPERTY(BlueprintAssignable)
	FOnDrawingClearSpellSuccessSignature OnDrawingClearSpellSuccessSignature;
	
	UFUNCTION(BlueprintCallable)
	TArray<FGameplayTag> GetRuneTags() const {return RuneTags;};
	
	UFUNCTION(BlueprintCallable)
	void SetIsStartFocus(bool bStartFocus) { bIsStartFocus = bStartFocus;};
	
	//Bind Tag When GameplayAbilities Change
	UFUNCTION(BlueprintCallable,Category=Rune)
	void SetBindOnSpecificTagChanged(UAbilitySystemComponent* AbilitySystemComponent);
	UFUNCTION(BlueprintImplementableEvent,Category=Rune)
	void OnSpecificTagChanged(const FGameplayTag Tag, int32 NewCount);

private:

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Rune,meta=(AllowPrivateAccess=true))
	bool bIsDrawToUpgraded = false;
	
	void CheckDrawSpell();
	bool bIsStartFocus=false;
	bool bIsDrawing=false;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Player,meta=(AllowPrivateAccess=true))
	TObjectPtr<AMainPlayerController> MainPlayerController;

	//Initial Tag Lists
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Rune,meta=(AllowPrivateAccess=true))
	TArray<FAbilitiesTagList> RuneAbilitiesTagMatchesLists;

	//Save Tag From Drawing
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Rune,meta=(AllowPrivateAccess=true))
	TArray<FGameplayTag> RuneTags;

	
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
