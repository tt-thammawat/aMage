// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MainInputConfig.generated.h"

struct FGameplayTag;

//FindTags With Input
USTRUCT(BlueprintType)
struct FMainInputAction
{
	GENERATED_BODY()

	//Input
	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction=nullptr;

	//Tag
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ActionInputTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class AMAGE_API UMainInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag,bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FMainInputAction> AbilitiesInputActions;
	
};
