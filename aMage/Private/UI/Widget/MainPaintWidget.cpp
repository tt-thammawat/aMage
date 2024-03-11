// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/MainPaintWidget.h"

#include "GameplayTagsSingleton.h"
#include "MainAssetManager.h"
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
			RemoveAllPoints();
		}
	else
		{
			if (Result.NameTag.MatchesTagExact(FMainGameplayTags::Get().Rune_Tag_04_Clear))
			{
				OnClearSpellSuccess.Broadcast();
			}
			else
			{
				RuneTags.Add(Result.NameTag);
				if(!RemoveLastRuneTagIfNotMatch())
				{
					OnDrawingRuneSuccess.Broadcast();
				}
			}
			RemoveAllPoints();
		}
}

void UMainPaintWidget::SetUpMainPlayerController(APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is nullptr in UMainPaintWidget::SetUpMainPlayerController"));
		return;
	}

	AMainPlayerController* CastedController = Cast<AMainPlayerController>(PlayerController);
	if (!CastedController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController could not be cast to AMainPlayerController in UMainPaintWidget::SetUpMainPlayerController"));
		return;
	}

	MainPlayerController = CastedController;
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
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && bIsStartFocus)
	{
		bIsDrawing = false;
		CheckDrawSpell();
		return FReply::Handled();
	}
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

bool UMainPaintWidget::RemoveLastRuneTagIfNotMatch()
{
	if (RuneTags.IsEmpty())
	{
		return false; // Exit if there are no tags to validate, returning false.
	}
    
	FGameplayTag LastTag = RuneTags[RuneTags.Num() - 1];
    FGameplayTag SecondLastTag = (RuneTags.Num() > 1) ? RuneTags[RuneTags.Num() - 2] : FGameplayTag();
	
	bool bTagFound = false;

	// Iterate over each tag mapping.
	for (const FAbilitiesTagList& TagMapping : RuneAbilitiesTagMatchesLists)
	{
		// Ensure we don't access an index out of bounds.
        if (TagMapping.Tags.Num() >= RuneTags.Num())
		{
        	bool bLastTagMatches = TagMapping.Tags[RuneTags.Num() - 1] == LastTag;
        	bool bSecondLastTagMatches = SecondLastTag.IsValid() ? TagMapping.Tags[RuneTags.Num() - 2] == SecondLastTag : true;

			// Check if the last tag in RuneTags matches the tag at the corresponding index in TagMapping.Tags.
            if (bLastTagMatches && bSecondLastTagMatches)
			{
				bTagFound = true;
				break;  // Exit the loop early since we've found a match.
			}
		}
	}
    
	// If the tag wasn't found at the corresponding index in any of the tag lists, remove it.
	if (!bTagFound)
	{
		RuneTags.RemoveAt(RuneTags.Num() - 1);
		return true; // Return true because a tag was removed.
	}

	return false; // Return false as no tag was removed.
}

void UMainPaintWidget::K2_CallClearSpellFunction()
{
	OnClearSpellSuccess.Broadcast();
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
