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

UCLASS()
class AMAGE_API UBaseRuneDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	//TMap To Match Class And Attributes Struct
	UPROPERTY(EditDefaultsOnly,Category = "Base Rune")
	TMap<ERuneType,FBaseItemTypeInfo> BaseRuneTypeInformation;
	UFUNCTION(BlueprintCallable,Category = "Base Rune")
	FBaseItemTypeInfo GetClassDefaultInfo(ERuneType RuneType);

};
