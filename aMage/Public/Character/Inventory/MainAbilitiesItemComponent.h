// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Data/BaseRuneDataAsset.h"
#include "Components/AGR_ItemComponent.h"
#include "MainAbilitiesItemComponent.generated.h"

class UMainInputAction;
class UGameplayAbility;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup=("AGR"), meta=(BlueprintSpawnableComponent))
class AMAGE_API UMainAbilitiesItemComponent : public UAGR_ItemComponent
{
	GENERATED_BODY()
	
public:
	//Initial Item Base
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category="AGR|Base Info")
	TObjectPtr<UTexture2D> ItemIcon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category="AGR|Base Info")
	FText ItemDescription;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category="AGR|Base Info")
	FName ItemSocketName;
	
	//Initial Item Special Abilities
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category="AGR|Base02 Info")
	FText AbilitiesDescriptions;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category="AGR|Base02 Info")
	bool bHaveAbilityWidget=false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category="AGR|Base02 Info")
	TArray<TSubclassOf<UGameplayAbility>> ItemAbilities;
	
};
