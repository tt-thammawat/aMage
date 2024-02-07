#pragma once
#include "GameplayTagContainer.h"
#include "ItemStruct.generated.h"

//Will Fetch Texture2d And Detail From Singleton
USTRUCT(BlueprintType)
struct FItemIconDescription
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName IconId; // Reference to a centralized icon repository

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DescriptionId; // Reference to a centralized text repository for descriptions
};

USTRUCT(BlueprintType)
struct FItemDetail
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Localization")
	FText DetailName; // Localized name for the detail

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Localization")
	FItemIconDescription IconAndDescription; // Using the normalized structure
};

USTRUCT(BlueprintType)
// This Data Include Basic Information + Abilities
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ItemTag; // For identification

	//[0] Item Main Name and Item Texture2d Main
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Details")
	TArray<FItemDetail> ListOfDetails; // Using the revised detail structure

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> ItemAbilities; // No change here
	
};
