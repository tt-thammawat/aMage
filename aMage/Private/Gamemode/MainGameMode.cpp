// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/MainGameMode.h"
#include "EngineUtils.h"
#include "Actor/Spawn/ASpawnManager.h"
#include "GameFramework/PlayerState.h"
#include "GameState/MainGameState.h"
#include "Service/SpawnService.h"

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(!MainGameState)
	{
		MainGameState = GetGameState<AMainGameState>();
	}
	
	for (TActorIterator<AASpawnManager> It(GetWorld()); It; ++It)
	{
		AASpawnManager* FoundSpawnManager = *It;
		if (FoundSpawnManager)
		{
			SpawnManager = FoundSpawnManager;
			break;
		}
	}

}

AMainGameMode::AMainGameMode()
{
	GameStateClass = AMainGameState::StaticClass();

}

void AMainGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(!MainGameState)
	{
		MainGameState = GetGameState<AMainGameState>();
	}
	

	if (const APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>())
	{
		const FString PlayerName = PlayerState->GetPlayerName();
		const int32 UniqueID = PlayerState->GetUniqueID();
        
		MainGameState->InitPlayerInfo(UniqueID, PlayerName);
	}
}

void AMainGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

int32 AMainGameMode::GetCurrentPlayerCount() const
{
	return MainGameState->PlayerArray.Num();
}

void AMainGameMode::StartNewWave()
{
	if(CurrentWave <= MaxWave)
	{
		CurrentWave++;

		if (MainGameState)
		{
			MainGameState->SetCurrentWave(CurrentWave);
			MainGameState->SetTimeBeforeSpawnWaves(0);
		}
	
		OnStartWave.Broadcast(true);
	 
		if (SpawnManager && SpawnManager->GetClass()->ImplementsInterface(USpawnService::StaticClass()))
		{
			ISpawnService::Execute_DeleteAllChest(SpawnManager);
			ISpawnService::Execute_DeleteAllStaff(SpawnManager);

			const int32 Difficulty =BaseDifficulty * GetCurrentPlayerCount();;
		
			WaveIncrement += Difficulty * FMath::Pow(GrowthFactor, CurrentWave);
    
			int32 EnemiesPerWaves = BaseEnemiesPerWave + WaveIncrement;
		
			CurrentEnemies = EnemiesPerWaves;
		
			if (MainGameState)
			{
				MainGameState->SetEnemiesThisWave(CurrentEnemies);
			}
		
			EnemiesPerWaves = FMath::Clamp(EnemiesPerWaves, BaseEnemiesPerWave, 64);
		
			ISpawnService::Execute_SpawnEnemies(SpawnManager,GetCurrentWave(), EnemiesPerWaves);
		}
	}
}

void AMainGameMode::WaitBeforeStartNewWaves(bool bIsFirstTime)
{
	if(bIsFirstTime == false)
	{
		//Spawn Chest
		SpawnChest();
		SpawnStaff();
	}
	
	if(MainGameState)
	{
		MainGameState->SetTimeBeforeSpawnWaves(TimeBeforeSpawnWaves);
	}
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_WaitBeforeStartWaves, this, &AMainGameMode::StartNewWave, TimeBeforeSpawnWaves, false);
}

void AMainGameMode::SpawnChest()
{
	if (SpawnManager && SpawnManager->GetClass()->ImplementsInterface(USpawnService::StaticClass()))
	{
		const int32 PlayerCount = GetCurrentPlayerCount();
    
		ISpawnService::Execute_SpawnChests(SpawnManager,CurrentWave, BaseDifficulty + PlayerCount);
	}
}

void AMainGameMode::SpawnStaff()
{
	if (SpawnManager && SpawnManager->GetClass()->ImplementsInterface(USpawnService::StaticClass()))
	{
		const int32 PlayerCount = GetCurrentPlayerCount();
        
		ISpawnService::Execute_SpawnStaffs(SpawnManager,CurrentWave, BaseDifficulty + PlayerCount);
	}
}

void AMainGameMode::EnemyKilled(const AActor* InstigatorActor)
{
	CurrentEnemies--;

	if (MainGameState)
	{
		MainGameState->SetEnemiesThisWave(CurrentEnemies);
		MainGameState->SetWhoKilled(InstigatorActor);
	}
	
	if (CurrentEnemies <= 0)
	{
		if (CurrentWave >= MaxWave)
		{
			MainGameState->EndGame();
		}
		else
		{
			OnStartWave.Broadcast(false);
			WaitBeforeStartNewWaves();
		}
	}
}

void AMainGameMode::PlayerDead(const AActor* DeadPlayer)
{
	if(MainGameState)
	{
		MainGameState->SetWhoDead(DeadPlayer);
	}
}


EItemRarity AMainGameMode::GenerateItemRarity()
{
	// Simplified example of generating a rarity based on weighted randomization
	float RarityRoll = FMath::RandRange(0.f, 100.f);

	if (RarityRoll <= 50.f) // 50% chance
		return EItemRarity::Common;
	else if (RarityRoll <= 75.f) // 25% chance
		return EItemRarity::Uncommon;
	else if (RarityRoll <= 90.f) // 15% chance
		return EItemRarity::Rare;
	else if (RarityRoll <= 97.f) // 7% chance
		return EItemRarity::Epic;
	else // 3% chance
		return EItemRarity::Legendary;
}

ERuneType AMainGameMode::RandomRuneType()
{
	// Subtract 1 because we're using 'Max' as a non-selectable value
	int32 MaxEnumValue = static_cast<int32>(ERuneType::Max) - 1;

	// Generate a random index within the range of your actual enum values
	int32 RandomIndex = FMath::RandRange(0, MaxEnumValue);

	// Cast the random index back to your enum type
	ERuneType RandomRuneType = static_cast<ERuneType>(RandomIndex);

	return RandomRuneType;
	
}