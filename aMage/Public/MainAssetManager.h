// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MainAssetManager.generated.h"

struct FUnistrokeRecognizer;
/**
 * 
 */
UCLASS()
class AMAGE_API UMainAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UMainAssetManager& Get();
protected:
	virtual void StartInitialLoading() override;
	virtual void PostInitialAssetScan() override;
	// Initialize the recognizer system
	void InitializeRecognizerSystem();
	void OnStokeDataTableLoaded();

private:
	TSoftObjectPtr<UDataTable> UnistrokeTemplatesTable;
	TSharedPtr<FUnistrokeRecognizer> Recognizer;

public:
	TSharedPtr<FUnistrokeRecognizer> GetRecognizer() const;
	
};
