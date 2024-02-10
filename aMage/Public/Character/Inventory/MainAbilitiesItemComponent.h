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
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	//Initial Item Base LMB Abilities
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category="AGR|Base Info")
	TObjectPtr<UTexture2D> ItemIcon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category="AGR|Base Info")
	FText ItemDescription;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category="AGR|Base Info")
	TObjectPtr<UMainInputAction> BaseAbilityLMBAction;

	//Initial Item Special Abilities
	//This Info Is for Checking with Drawing Canvas
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category="AGR|Base02 Info")
	TArray<FRuneMatchingInfo> MatchingInfo;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category="AGR|Base02 Info")
	FText AbilitiesDescriptions;
	//This is like mana available for spell when use it reduce if == 0 need to draw again
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category="AGR|Base02 Info")
	float UsageTimes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category="AGR|Base Info")
	TObjectPtr<UMainInputAction> SpecialAbilityLMBAction;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category="AGR|Base Info")
	TObjectPtr<UMainInputAction> SpecialAbilityRMBAction;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category="AGR|Base02 Info")
	TArray<TSubclassOf<UGameplayAbility>> ItemAbilities;
	
};
