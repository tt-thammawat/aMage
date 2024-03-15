// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AMAGE_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Item Data Management")
	UTexture2D* GetSpellIconTexture(FName RuneName);

	UFUNCTION(BlueprintCallable, Category="Item Data Management")
	FText GetSpellDescription(FName DescriptionName);
protected:
	
	UPROPERTY(EditAnywhere, Category="Data")
	TMap<FName, UTexture2D*> SpellTextures;

	UPROPERTY(EditAnywhere, Category="Data")
	TMap<FName, FText> SpellDescriptions;
private:
	
};
