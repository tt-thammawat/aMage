// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/MainPaintWidget.h"

#include "GameplayTagsSingleton.h"
#include "MainAssetManager.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/MainPlayerController.h"
#include "DrawMagic/UnistrokeRecognizer.h"
#include "DrawMagic/UnistrokeResult.h"

void UMainPaintWidget::NativeConstruct()
{
	Super::NativeConstruct();
	bIsDrawing=false;
	bIsStartFocus=false;
}

void UMainPaintWidget::CheckDrawSpell()
{
	const TArray<FVector2D>CurrentPoints = Points;
	const FUnistrokeResult Result = UMainAssetManager::Get().GetRecognizer()->Recognize(CurrentPoints, false);
	
	if (Result.Score < 0.8f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "No Magic", true, FVector2D(1, 1));
			RemoveAllPoints();
		}
	else
		{
			if (Result.NameTag.MatchesTagExact(FMainGameplayTags::Get().Rune_Tag_04_Clear))
			{
				OnDrawingClearSpellSuccessSignature.Broadcast();
			}
			else
			{
				RuneTags.Add(Result.NameTag);
				OnDrawingSpellSuccess.Broadcast();
			}


			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("MaGic %s"), *Result.NameTag.GetTagName().ToString()));
			RemoveAllPoints();
		}
}

void UMainPaintWidget::SetUpMainPlayerController(APlayerController* PlayerController)
{
	MainPlayerController = Cast<AMainPlayerController>(PlayerController);
	if(MainPlayerController->GetBaseAbilitySystemComponent())
	{
		MainPlayerController->GetBaseAbilitySystemComponent()->RegisterGameplayTagEvent(FMainGameplayTags::Get().State_Action_Spell_NormalSpell,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&ThisClass::OnSpecificTagChanged);
	}
}

FReply UMainPaintWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsDrawing = true;
		//start a new line segment
		//LineSegments.Add(TArray<FVector2D>());
		return FReply::Handled();
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UMainPaintWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsDrawing && MainPlayerController->GetIsDrawingSpell() && bIsStartFocus)
	{
		FVector2D LocalMousePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		AddPoint(LocalMousePosition);
		return FReply::Handled();

	}
	
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UMainPaintWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsDrawing = false;
		CheckDrawSpell();
		return FReply::Handled();
	}
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

//Drawing Canvas Line
int32 UMainPaintWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                    const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	UWidgetBlueprintLibrary::DrawLines(Context, Points, FLinearColor::White, true, 10.0f);

	// for (const TArray<FVector2D>& Segment : LineSegments)
	// {
	// 	if (Segment.Num() > 1) // Ensure we have at least two points to draw a line
	// 	{
	// 		FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	// 		UWidgetBlueprintLibrary::DrawLines(Context, Segment, FLinearColor::Blue, true, 10.0f);
	// 	}
	// }

	return LayerId + 1;
}

void UMainPaintWidget::AddPoint(const FVector2D& Point)
{
	// // Add the point to the current drawing session for visual line drawing
	// if (LineSegments.Num() == 0 || !bIsDrawing)
	// {
	// 	LineSegments.Add(TArray<FVector2D>()); // Start a new line segment if necessary
	// }
	// LineSegments.Last().Add(Point);

	// Also add the point to Points for spell recognition
	Points.Add(Point);
}

void UMainPaintWidget::RemoveAllPoints()
{
	Points.Empty();
//	LineSegments.Empty();
}
