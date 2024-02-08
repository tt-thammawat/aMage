// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AGR_ItemComponent.h"
#include "MainAbilitiesItemComponent.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup=("AGR"), meta=(BlueprintSpawnableComponent))
class AMAGE_API UMainAbilitiesItemComponent : public UAGR_ItemComponent
{
	GENERATED_BODY()
protected:
	
public:
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category="AGR|Base Info")
//	FItemData ItemData;

	//TODO:Place Holder for Item Detail May Use Item Struct To Form The Item Rarities
	// - Rarity 1-3 = RuneDrawing Icon , Descriptioption , 
	//UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Item Details")
	
	
};
