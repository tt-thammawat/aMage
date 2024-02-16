// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MainGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AMAGE_API UMainGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	
public:
	// Input Button Tag
	UPROPERTY(EditDefaultsOnly,Category = "Input")
	FGameplayTag StartupInputTag;
};