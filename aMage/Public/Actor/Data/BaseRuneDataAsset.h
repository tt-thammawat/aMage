// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Actor/Data/BaseItemInfo.h"
#include "BaseRuneDataAsset.generated.h"

class UMainInputAction;
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
struct FRuneMatchingInfo
{
	GENERATED_BODY()

	//TODO : Will Change This in to Point Struct From Data Table Later
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	FName MatchingName;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	bool bIsUnlockSpecialAbility=false;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	TObjectPtr<UTexture2D> SpecialAbilityIcon;
};

USTRUCT(BlueprintType)
struct FRuneSpecialInput
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	TObjectPtr<UMainInputAction> SpecialAbilityLMBAction;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	TObjectPtr<UMainInputAction> SpecialAbilityRMBAction;
};

USTRUCT(BlueprintType)
struct FRuneSpecialAbility
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	TArray<FRuneMatchingInfo> MatchingInfos;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	FText SpecialAbilitiesDescription;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	float UsageTime;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	TSubclassOf<UGameplayAbility> SpecialAbilities;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	FRuneSpecialInput SpecialAbilitiesInput;
};


USTRUCT(BlueprintType)
struct FRuneSpecialAbilitiesList
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rune")
	TArray<FRuneSpecialAbility> RuneCastingAbilitiesInfo;
};

UCLASS()
class AMAGE_API UBaseRuneDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	//Base Left Click Ability
	UPROPERTY(EditDefaultsOnly,Category = "Base Rune")
	TMap<ERuneType, FBaseItemTypeInfoList> RuneTypeInformation;
	UFUNCTION(BlueprintCallable,Category = "Base Rune")
	FBaseItemTypeInfo GetClassDefaultInfo(ERuneType RuneType);
	//Special Right Click Ability
	UPROPERTY(EditDefaultsOnly,Category="Rune SpecialAbilities")
	TMap<ERuneType,FRuneSpecialAbilitiesList> RuneSpecialAbilitiesInformation;
	UFUNCTION(BlueprintCallable,Category="Rune SpecialAbilities")
	FRuneSpecialAbility GetRuneAbilitiesInfo(ERuneType RuneType);

};
