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
				//true LastTag From RuneTag is remove : false No Tag Is Remove
				if(CheckIfRuneTagMatchTagList() == true)
				{
					OnDrawingRuneSuccess.Broadcast();
				}
				else if(CheckIfRuneTagMatchTagList() == false)
				{
					RuneTags.RemoveAt(RuneTags.Num() - 1);
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

bool UMainPaintWidget::CheckIfRuneTagMatchTagList()
{
	if (RuneTags.IsEmpty())
	{
		return true; 
	}

	for (const FAbilitiesTagList& TagMapping : RuneAbilitiesTagMatchesLists)
	{
		if (TagMapping.Tags.Num() < RuneTags.Num())
		{
			continue; 
		}
		bool bAllTagsMatch = true;
		for (int32 i = 0; i < RuneTags.Num(); ++i)
		{
			if (TagMapping.Tags[i] != RuneTags[i])
			{
				bAllTagsMatch = false; 
				break; 
			}
		}

		if (bAllTagsMatch)
		{
			return true;
		}
	}
	return false;
}

void UMainPaintWidget::K2_CallClearSpellFunction()
{
	OnClearSpellSuccess.Broadcast();
}

TArray<FGameplayTag> UMainPaintWidget::GetExtendedUniqueTagMappings()
{
	TArray<FGameplayTag> UniqueExtendedTags;

	// Special case when there are no RuneTags: return the first unique tag from each TagMapping's first position.
	if (RuneTags.IsEmpty())
	{
		for (const FAbilitiesTagList& TagMapping : RuneAbilitiesTagMatchesLists)
		{
			if (!TagMapping.Tags.IsEmpty())
			{
				FGameplayTag FirstTag = TagMapping.Tags[0];
				UniqueExtendedTags.AddUnique(FirstTag);
			}
		}
	}
	else
	{
		for (const FAbilitiesTagList& TagMapping : RuneAbilitiesTagMatchesLists)
		{
			// Check if TagMapping has enough tags to be an extension of RuneTags.
			if (TagMapping.Tags.Num() > RuneTags.Num())
			{
				bool bPrefixMatches = true;
				// Verify the prefix of TagMapping matches RuneTags.
				for (int32 i = 0; i < RuneTags.Num(); ++i)
				{
					if (TagMapping.Tags[i] != RuneTags[i])
					{
						bPrefixMatches = false;
						break; // Prefix does not match, move to the next TagMapping.
					}
				}

				if (bPrefixMatches)
				{
					// The next tag after the RuneTags sequence in the TagMapping.
					FGameplayTag NextTag = TagMapping.Tags[RuneTags.Num()];
					// Add this tag to the results if it's not already included.
					if (!UniqueExtendedTags.Contains(NextTag))
					{
						UniqueExtendedTags.Add(NextTag);
					}
				}
			}
		}
	}

	return UniqueExtendedTags;

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
