// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Data/BaseRuneDataAsset.h"
#include "..\Actor\Data\BaseItemInfo.h"
#include "GameFramework/GameMode.h"
#include "MainGameMode.generated.h"

class UBaseRuneDataAsset;
class UCharacterClassInfo;



/**
 * 
 */
UCLASS()
class AMAGE_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassDefaultInfo;

	UFUNCTION(BlueprintCallable,Category ="Generate Rune")
	EItemRarity GenerateItemRarity();
	UFUNCTION(BlueprintCallable,Category="Rune")
	void SpawnItemWithRarity();

	
	//Random Generate Base Rune Type
	UFUNCTION(BlueprintCallable,Category ="Generate Rune")
	ERuneType RandomBaseRuneType();
	//use ERuneType to get FBaseRuneTypeInfo from DataAsset
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Rune")
	TObjectPtr<UBaseRuneDataAsset> BaseRuneTypeInfo;

	
	


	

};
