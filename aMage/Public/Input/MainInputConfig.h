// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MainInputConfig.generated.h"

/**
 * 
 */


struct FGameplayTag;

//Create This Struct TO Match The InputAction With InputTag and use it to know which button was pressed and need to pressed to activated
//ablilities that have the same input tag
USTRUCT(BlueprintType)
struct FGMainInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction=nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ActionInputTag = FGameplayTag();
	
};

UCLASS()
class AMAGE_API UMainInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:

	//return Input Action That Associate with FGameplayTags
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag,bool bLogNotFound = false) const;
	
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FGMainInputAction> AbilitiesInputActions;
};
