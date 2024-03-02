// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Spawn/ASpawnManager.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AASpawnManager::AASpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AASpawnManager::StartSpawningWaves()
{
	SpawnWave();

}

void AASpawnManager::SpawnWave()
{
	if(!HasAuthority()) return;
	
	if (GoblinClass == nullptr || SpawnPoints.Num() == 0) return;

	for (int32 i = 0; i < GoblinsPerWave; ++i)
	{
		int32 SpawnPointIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
		FVector SpawnLocation = UKismetMathLibrary::TransformLocation(GetActorTransform(),SpawnPoints[SpawnPointIndex]);
		FRotator SpawnRotation = FRotator::ZeroRotator;
		GetWorld()->SpawnActor<AActor>(GoblinClass, SpawnLocation,SpawnRotation);
	}

	++CurrentWave;

	// Schedule the next wave
	GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &AASpawnManager::SpawnWave, TimeBetweenWaves, false);
}

