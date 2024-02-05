// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/MainPaintWidget.h"
#include "MainAssetManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/MainPlayerController.h"
#include "DrawMagic/UnistrokeRecognizer.h"
#include "DrawMagic/UnistrokeResult.h"
#include "Engine/UserInterfaceSettings.h"

void UMainPaintWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(MainPlayerController->GetIsDrawingSpell())
	{
		StartDrawing();
	}
	
}

void UMainPaintWidget::CheckDrawSpell()
{
	const TArray<FVector2D>CurrentPoints = GetPoints();
	const FUnistrokeResult Result = UMainAssetManager::Get().GetRecognizer()->Recognize(CurrentPoints, false);
	
	if (Result.Score < 0.8f)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Magic", true, FVector2D(2, 2));
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Result.Name, true, FVector2D(2, 2));
		//TODO::Add Success Casting Condition Here  Here
	}
	
	RemoveAllPoints();
}

void UMainPaintWidget::StartDrawing()
{
	float MouseX = 0.0f;
	float MouseY = 0.0f;
	
	GetOwningPlayer()->GetMousePosition(MouseX, MouseY);
		
	const FVector2D MousePoint = FVector2D(MouseX, MouseY);
	const FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	const float viewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportSize.X, viewportSize.Y));		
	const FVector2D ScaledPoint = MousePoint / viewportScale;
	const FVector2D LastPoint = GetPoints().Num() > 0 ? GetPoints().Last() : ScaledPoint;
	const bool IsNewPoint = !LastPoint.Equals(ScaledPoint, 1.0f);
		
	if (Points.Num() == 0 || (Points.Num() > 0 && IsNewPoint))
	{
		AddPoint(ScaledPoint);
	}
}

void UMainPaintWidget::SetUpMainPlayerController()
{
	MainPlayerController = Cast<AMainPlayerController>(GetOwningPlayer());
}


//Drawing Canvas Line
int32 UMainPaintWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                    const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                    const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	UWidgetBlueprintLibrary::DrawLines(Context, Points, FLinearColor::Blue, true, 10.0f);

	return LayerId + 1;
}

TArray<FVector2D> UMainPaintWidget::GetPoints()
{
	return Points;
}

void UMainPaintWidget::AddPoint(const FVector2D& Point)
{
	Points.Add(Point);

}

void UMainPaintWidget::RemoveAllPoints()
{
	Points.Empty();
}
