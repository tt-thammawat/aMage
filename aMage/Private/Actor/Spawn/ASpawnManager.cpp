// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Spawn/ASpawnManager.h"

#include "Character/BaseEnemy.h"
#include "Gamemode/MainGameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Service/ServiceLocator.h"

// Sets default values
AASpawnManager::AASpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;
	
}

AASpawnManager::~AASpawnManager()
{
	FServiceLocator::Get().DeregisterService("SpawnManager");
}

void AASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	FServiceLocator::Get().RegisterService("SpawnManager",this);
}

bool AASpawnManager::EnoughPlayerPressedStart(AActor* PressingActor)
{
	if (!PressingActor) return false;

	int32 PlayerId = PressingActor->GetUniqueID();
	PlayersPressedStart.Emplace(PlayerId);
	
	AMainGameMode* GM = GetWorld()->GetAuthGameMode<AMainGameMode>();
    if (GM && PlayersPressedStart.Num() >= GM->GetCurrentPlayerCount())
	{
		GM->WaitBeforeStartNewWaves(true);
    	return true;
	}
	
	return false;
}

void AASpawnManager::DelayedSpawnEnemy(ECharacterClass EnemyType, FVector SpawnLocation, FRotator SpawnRotation)
{
	if (TSubclassOf<ABaseEnemy>* EnemyClass = EnemyToSpawnClass.Find(EnemyType))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		GetWorld()->SpawnActor<ABaseEnemy>(*EnemyClass, SpawnLocation, SpawnRotation,SpawnParams);
	}
}

void AASpawnManager::DeleteAllChest_Implementation()
{
	if(SpawnChestArray.IsEmpty()) return;

	for(const TObjectPtr<AActor>& Chest : SpawnChestArray)
	{
		Chest->Destroy();
	}

	SpawnChestArray.Empty();
}

void AASpawnManager::SpawnEnemies_Implementation(int32 CurrentWaves,int32 BatchEnemiesSpawn)
{
	if(!HasAuthority()) return;
	
	if (EnemyToSpawnClass.IsEmpty() || EnemySpawnPoints.IsEmpty()) return;
	
	TArray<ECharacterClass> SpawnableTypes;
	
	if (CurrentWaves >= 1 && CurrentWaves <= 2)
	{
		SpawnableTypes.Add(ECharacterClass::Warrior);
	}
	else if (CurrentWaves >= 3 && CurrentWaves <= 4)
	{
		SpawnableTypes.Add(ECharacterClass::Warrior);
		SpawnableTypes.Add(ECharacterClass::Ranger);
	}
	else if (CurrentWaves >= 5)
	{
		SpawnableTypes.Append({ECharacterClass::Wizard, ECharacterClass::Warrior, ECharacterClass::Ranger});
	}
	
	for (int32 i = 0; i < BatchEnemiesSpawn; ++i)
	{
		if (SpawnableTypes.IsEmpty()) continue;

		int32 TypeIndex = FMath::RandRange(0, SpawnableTypes.Num() - 1);
		ECharacterClass SelectedType = SpawnableTypes[TypeIndex];
		
		int32 SpawnPointIndex = FMath::RandRange(0, EnemySpawnPoints.Num() - 1);
		FVector SpawnLocation = UKismetMathLibrary::TransformLocation(GetActorTransform(), EnemySpawnPoints[SpawnPointIndex]);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		
		float SpawnDelay = FMath::FRandRange(1.5f, 6.0f);

		FTimerDelegate TimerDel;
		FTimerHandle WaveTimerHandle;
		
		TimerDel.BindUFunction(this, FName("DelayedSpawnEnemy"), SelectedType, SpawnLocation, SpawnRotation);
		
		GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, TimerDel, SpawnDelay, false);
	}

}

void AASpawnManager::SpawnChests_Implementation(int32 CurrentWaves,int32 Difficulty)
{
	TArray<FVector> TempChestSpawnPoints = ChestsSpawnPoints;

	int32 BaseChestsToSpawn = FMath::Clamp(CurrentWaves / Difficulty, 1, ChestsSpawnPoints.Num());
	int32 MaxChestsCanSpawn = TempChestSpawnPoints.Num();
	ChestsToSpawn = FMath::Min(BaseChestsToSpawn, MaxChestsCanSpawn);
	
	for(int32 i = 0; i < ChestsToSpawn; ++i)
	{
		if (TempChestSpawnPoints.Num())
		{
			const int32 PointIndex = FMath::RandRange(0, TempChestSpawnPoints.Num() - 1);
			
			FVector SpawnLocation = UKismetMathLibrary::TransformLocation(GetActorTransform(),TempChestSpawnPoints[PointIndex]);
			FRotator SpawnRotation = FRotator::ZeroRotator;

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			AActor* ChestActor = GetWorld()->SpawnActor<AMainInteractActor>(ChestClass, SpawnLocation, SpawnRotation, SpawnParams);
			
			SpawnChestArray.Add(ChestActor);
			
			TempChestSpawnPoints.RemoveAtSwap(PointIndex, 1, false);
		}
	}
}

int32 AASpawnManager::GetCurrentEnemyCount_Implementation() const
{
	return ISpawnService::GetCurrentEnemyCount_Implementation();
}



