// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Actor/InteractActor/MainInteractActor.h"
#include "Service/SpawnService.h"
#include "ASpawnManager.generated.h"

class ABaseEnemy;


UCLASS()
class AMAGE_API AASpawnManager : public AMainInteractActor,public ISpawnService
{
	GENERATED_BODY()

	
public:	
	AASpawnManager();
	~AASpawnManager();

	void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	bool EnoughPlayerPressedStart(AActor* PressingActor);

	UFUNCTION()
	void DelayedSpawnEnemy(ECharacterClass EnemyType, FVector SpawnLocation, FRotator SpawnRotation);

	
	//Start ISpawnService Interface
	void SpawnEnemies_Implementation(int32 CurrentWaves,int32 BatchEnemiesSpawn) override;
	void SpawnChests_Implementation(int32 CurrentWaves,int32 Difficulty) override;
	void DeleteAllChest_Implementation() override;

	int32 GetCurrentEnemyCount_Implementation() const override;
	//End ISpawnService


private:
	TSet<uint32> PlayersPressedStart;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Management",meta=(MakeEditWidget="true"))
	TArray<FVector> EnemySpawnPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Management")
	TMap<ECharacterClass,TSubclassOf<ABaseEnemy>> EnemyToSpawnClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Management")
	float TimeBeforeSpawnWaves = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Management")
	TSubclassOf<AMainInteractActor> ChestClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Management",meta=(MakeEditWidget="true"))
	TArray<FVector> ChestsSpawnPoints;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave Management")
	int32 ChestsToSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave Management")
	TArray<TObjectPtr<AActor>> SpawnChestArray;
	
	FTimerHandle TimerHandle_WaitBeforeStartWaves;
	
};
