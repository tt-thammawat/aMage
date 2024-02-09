#pragma once
#include "GameplayTagContainer.h"
#include "BaseItemInfo.generated.h"


class UGameplayAbility;

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Common UMETA(DisplayName = "Common"),
	Uncommon UMETA(DisplayName = "Uncommon"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary"),
	Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FBaseItemTypeInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	FName ItemName;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	FGameplayTag ItemTag;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	TObjectPtr<UTexture2D> ItemIcon;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	FText ItemDescription;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	TObjectPtr<UStaticMesh> ItemMeshComponent;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	TSubclassOf<UGameplayAbility> ItemBaseAbilities;
};

USTRUCT(BlueprintType)
struct FBaseItemTypeInfoList
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	TArray<FBaseItemTypeInfo> ItemInfo;
};