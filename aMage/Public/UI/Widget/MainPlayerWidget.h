// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainPlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class AMAGE_API UMainPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	UFUNCTION(BlueprintCallable)
	float InterpolateProgress(float CurrentPercent, float TargetPercent, float DeltaTime, float InterpSpeed);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
private:
	
};
