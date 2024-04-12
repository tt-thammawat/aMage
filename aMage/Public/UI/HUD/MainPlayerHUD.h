// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainPlayerHUD.generated.h"

class AMainPlayerController;
class UMainPaintWidget;
/**
 * 
 */
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UMainPlayerWidget;

UCLASS()
class AMAGE_API AMainPlayerHUD : public AHUD
{
	GENERATED_BODY()
public:
	UFUNCTION()
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams); 
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UMainPlayerWidget> OverlayWidget;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UMainPlayerWidget> InventoryWidget;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UMainPlayerWidget> ScoreBoardWidget;
	
	void InitOverlay(APlayerController* PC,APlayerState* PS,UAbilitySystemComponent* ASC,UAttributeSet* AS);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OverlayWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InventoryWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ScoreBoardWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;



};
