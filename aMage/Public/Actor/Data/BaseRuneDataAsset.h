// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Actor/Data/BaseItemInfo.h"
#include "BaseRuneDataAsset.generated.h"

class UGameplayAbility;
/**
 * 
 */

UENUM(BlueprintType)
enum class ERuneType : uint8
{
	Fire UMETA(DisplayName = "FreRune"),
	Ice UMETA(DisplayName = "IceRune"),
	Electric UMETA(DisplayName = "Electric"),
	Max UMETA(Hidden)

};

USTRUCT(BlueprintType)
struct FCastingMatchingInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	FName CastingMatchingName;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	bool bIsUnlockCastingAbility=false;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	TObjectPtr<UTexture2D> CastingAbilityIcon;
};

USTRUCT(BlueprintType)
struct FRuneCastingAbilities
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	TArray<FCastingMatchingInfo> MatchingInfos;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	FText CastingAbilitiesDescription;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	TSubclassOf<UGameplayAbility> CastingSpecialBaseAbilities;
};

USTRUCT(BlueprintType)
struct FRuneCastingAbilitiesList
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	TArray<FRuneCastingAbilities> RuneCastingAbilitiesInfo;
};

UCLASS()
class AMAGE_API UBaseRuneDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	//Item Rarity
	UFUNCTION(BlueprintCallable,Category ="Generate Rune")
	EItemRarity GenerateItemRarity();
	//Random Generate Base Rune Type
	UFUNCTION(BlueprintCallable,Category ="Generate Rune")
	ERuneType RandomBaseRuneType();
	
	//Base Left Click Ability
	UPROPERTY(EditDefaultsOnly,Category = "Base Rune")
	TMap<ERuneType, FBaseItemTypeInfoList> BaseRuneTypeInformation;
	UFUNCTION(BlueprintCallable,Category = "Base Rune")
	FBaseItemTypeInfo GetClassDefaultInfo(ERuneType RuneType);
	//Special Right Click Ability
	UPROPERTY(EditDefaultsOnly,Category="Rune SpecialAbilities")
	TMap<ERuneType,FRuneCastingAbilitiesList> BaseRuneAbilitiesInformation;
	UFUNCTION(BlueprintCallable,Category="Rune SpecialAbilities")
	FRuneCastingAbilities GetBaseRuneAbilitiesInfo(ERuneType RuneType);

};
