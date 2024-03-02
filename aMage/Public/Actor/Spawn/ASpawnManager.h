// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASpawnManager.generated.h"

UCLASS()
class AMAGE_API AASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AASpawnManager();

protected:
	virtual void BeginPlay() override;

public:	
	// Function to start spawning waves of goblins
	UFUNCTION(BlueprintCallable, Category="Goblin Spawning")
	void StartSpawningWaves();

	// Template class for the goblin to spawn
	UPROPERTY(EditAnywhere, Category="Goblin Spawning")
	TSubclassOf<AActor> GoblinClass;

	// Points where goblins will be spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goblin Spawning",meta=(MakeEditWidget="true"))
	TArray<FVector> SpawnPoints;

	// Number of goblins per wave
	UPROPERTY(EditAnywhere, Category="Goblin Spawning")
	int32 GoblinsPerWave = 5;

	// Delay between waves
	UPROPERTY(EditAnywhere, Category="Goblin Spawning")
	float TimeBetweenWaves = 5.0f;

private:
	// Handles the spawning of a single wave
	void SpawnWave();

	// Timer handle for wave spawning
	FTimerHandle WaveTimerHandle;

	// Keeps track of the current wave number
	int32 CurrentWave = 0;
};
