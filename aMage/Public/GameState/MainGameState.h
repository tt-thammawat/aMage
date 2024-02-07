// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Inventory/ItemRarity.h"
#include "GameFramework/GameState.h"
#include "MainGameState.generated.h"

/**
 * 
 */
UCLASS()
class AMAGE_API AMainGameState : public AGameState
{
	GENERATED_BODY()

protected:
private:
	UFUNCTION(BlueprintCallable,Category="Spawn Item")
	void SpawnItemWithRarity();
	EItemRarity GenerateItemRarity();
};
