// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/MainPaintWidget.h"

#include "GameplayTagsSingleton.h"
#include "MainAssetManager.h"
#include "NiagaraSystemWidget.h"
#include "Abilities/GameplayAbility.h"
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
	
	if (Result.Score < 0.7f)
		{
			RemoveAllPoints();
			PlaySound(FailSound);
		}
	else
		{
			if (Result.NameTag.MatchesTagExact(FMainGameplayTags::Get().Rune_Tag_99_Clear))
			{
				OnClearSpellSuccess.Broadcast();
				LineNiagaraUISystemWidget->ActivateSystem(true);
				LineNiagaraUISystemWidget->DeactivateSystem();
				PlaySound(FailSound);
			}
			else if (Result.NameTag.MatchesTagExact(FMainGameplayTags::Get().Rune_Tag_99_Reload))
			{
				OnReloadSpellSuccess.Broadcast();
				PlaySound(ReloadSound);
			}
			else
			{
				RuneTags.Add(Result.NameTag);
				//true LastTag From RuneTag is remove : false No Tag Is Remove
				if(CheckIfRuneTagMatchInList() == true)
				{
					OnDrawingRuneSuccess.Broadcast();
					PlaySound(SuccessSound);

				}
				else if(CheckIfRuneTagMatchInList() == false)
				{
					RuneTags.RemoveAt(RuneTags.Num() - 1);
					PlaySound(FailSound);
				}
				else
				{
					PlaySound(FailSound);
				}
			}
			RemoveAllPoints();
		}
}

void UMainPaintWidget::SetUpMainPlayerController(APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is nullptr in SetUpMainPlayerController"));
		return;
	}

	AMainPlayerController* CastedController = Cast<AMainPlayerController>(PlayerController);
	if (!CastedController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController could not be cast to AMainPlayerController in SetUpMainPlayerController"));
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
		LineNiagaraUISystemWidget->ActivateSystem(false);
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
		LineNiagaraUISystemWidget->DeactivateSystem();
		CheckDrawSpell();
		return FReply::Handled();
	}
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

bool UMainPaintWidget::AreRuneTagsEqualInOrder(const TArray<FGameplayTag>& MappingTags)
{
	if (RuneTags.Num() != MappingTags.Num())
	{
		return false;
	}

	for (int32 i = 0; i < RuneTags.Num(); i++)
	{
		if (RuneTags[i] != MappingTags[i])
		{
			return false;
		}
	}
	return true;
}

void UMainPaintWidget::CheckCurrentlyActiveSpell(TSubclassOf<UGameplayAbility>& AbilityClass)
{
	for (const FAbilitiesTagList& TagMapping : RuneAbilitiesTagMatchesLists)
	{
    	if (AbilityClass == TagMapping.AbilityClass && !AreRuneTagsEqualInOrder(TagMapping.Tags))
		{
    		RuneTags = TagMapping.Tags;
    		ResetTagUI();
        	return;
		}
	}
}

bool UMainPaintWidget::CheckIfRuneTagMatchInList()
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
	PlaySound(FailSound);
	OnClearSpellSuccess.Broadcast();
	LineNiagaraUISystemWidget->DeactivateSystem();
}


void UMainPaintWidget::K2_CallReloadSpellFunction()
{
	PlaySound(ReloadSound);
	OnReloadSpellSuccess.Broadcast();
}

TArray<FHintTagMatch> UMainPaintWidget::SetHintTagMatchMapping()
{
    TArray<FHintTagMatch> ResultHints;


        for (const FAbilitiesTagList& TagMapping : RuneAbilitiesTagMatchesLists)
        {
	        if (TagMapping.Tags.Num() > RuneTags.Num())
	        {
	        	bool bPrefixMatches = true;
	        	for (int32 i = 0; i < RuneTags.Num(); ++i)
	        	{
	        		if (TagMapping.Tags[i] != RuneTags[i])
	        		{
	        			bPrefixMatches = false;
	        			break;
	        		}
	        	}

	        	if (bPrefixMatches)
	        	{
	        		FGameplayTag NextTag = TagMapping.Tags[RuneTags.Num()];
	        		FHintTagMatch NewHint;
	        		NewHint.Tag = NextTag;

	        		// Determine if the sequence matches exactly with TagMapping.
	        		NewHint.SpellMatchName = (TagMapping.Tags.Num() == RuneTags.Num() + 1) ? TagMapping.Name : FName();

	        		// Ensure the tag isn't already included.
	        		if (!ResultHints.ContainsByPredicate([&](const FHintTagMatch& Hint) { return Hint.Tag == NextTag; }))
	        		{
	        			ResultHints.Add(NewHint);
	        		}
	        	}
	        }
        }

    return ResultHints;
}

//Drawing Canvas Line
// int32 UMainPaintWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
//                                     const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
// {
// 	Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
// 	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
// 	UWidgetBlueprintLibrary::DrawLines(Context, Points, FLinearColor::White, true, 10.0f);
//
// 	// for (const TArray<FVector2D>& Segment : LineSegments)
// 	// {
// 	// 	if (Segment.Num() > 1) // Ensure we have at least two points to draw a line
// 	// 	{
// 	// 		FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
// 	// 		UWidgetBlueprintLibrary::DrawLines(Context, Segment, FLinearColor::Blue, true, 10.0f);
// 	// 	}
// 	// }
//
// 	return LayerId + 1;
// }

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
