#pragma once
#include "GameplayTagContainer.h"

#include "ItemStruct.generated.h"


class UEffectDataAsset;

USTRUCT(BlueprintType)
struct FItemData
{
	
	GENERATED_BODY()

	// TAG GRANT When PickUpItem
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ItemTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> ItemMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> ItemIconTexture;

	//Effect Store In This Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UEffectDataAsset*> ItemEffectDataAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemDurability;	
		
};