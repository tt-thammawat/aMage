// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpawnService.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,Blueprintable)
class USpawnService : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AMAGE_API ISpawnService
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void SpawnEnemies(int32 CurrentWaves,int32 BatchEnemiesSpawn);
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void SpawnChests(int32 CurrentWaves,int32 Difficulty);
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void DeleteAllChest();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	int32 GetCurrentEnemyCount() const;
};
