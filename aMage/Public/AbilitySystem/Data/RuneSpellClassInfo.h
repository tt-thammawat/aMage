// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "RuneSpellClassInfo.generated.h"


class UGameplayAbility;

USTRUCT(BlueprintType)
struct FRuneAbilityMapping
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FGameplayTagContainer RuneSpellTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> RuneSpellAbility;
};
/**
 * 
 */
UCLASS()
class AMAGE_API URuneSpellClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Rune Spell")
	TArray<FRuneAbilityMapping> AllRuneAbilities;

	UFUNCTION(BlueprintCallable,Category = "Rune Spell")
	TSubclassOf<UGameplayAbility> GetRuneSpellMatchingAbility(const FGameplayTagContainer& RuneTagContainer);
};
