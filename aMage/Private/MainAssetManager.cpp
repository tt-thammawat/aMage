// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAssetManager.h"

#include "GameplayTagsSingleton.h"
#include "DrawMagic/UnistrokeDataTable.h"
#include "DrawMagic/UnistrokeRecognizer.h"

UMainAssetManager& UMainAssetManager::Get()
{
	check(GEngine);
	UMainAssetManager* MainAssetManager=Cast<UMainAssetManager>(GEngine->AssetManager);
	return *MainAssetManager;
}

void UMainAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FMainGameplayTags::InitializeNativeGameplayTags();
}

void UMainAssetManager::PostInitialAssetScan()
{
	Super::PostInitialAssetScan();
	InitializeRecognizerSystem();

}

void UMainAssetManager::InitializeRecognizerSystem()
{
	//Construct Recognizer Drawing System
	Recognizer = MakeShareable(new FUnistrokeRecognizer());

	const FSoftObjectPath DataTablePath(TEXT("DataTable'/Game/MagicDrawing/RegconizaDataTable.RegconizaDataTable'"));
	UnistrokeTemplatesTable = TSoftObjectPtr<UDataTable>(DataTablePath);
	
	// Now load it asynchronously
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(UnistrokeTemplatesTable.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &UMainAssetManager::OnStokeDataTableLoaded));

}

void UMainAssetManager::OnStokeDataTableLoaded()
{
	if (UnistrokeTemplatesTable)
	{
		
		if (UnistrokeTemplatesTable != nullptr)
		{
			const FString ContextString = "Templates";
			TArray<FUnistrokeDataTable*> Rows;

			UnistrokeTemplatesTable->GetAllRows<FUnistrokeDataTable>(ContextString, Rows);

			for (const FUnistrokeDataTable* Row : Rows)
			{
				if (Row)
				{
					Recognizer->AddTemplate(Row->Name, Row->Points);
				}
			}
		}
	}
}

TSharedPtr<FUnistrokeRecognizer> UMainAssetManager::GetRecognizer() const
{
		return Recognizer;
}
