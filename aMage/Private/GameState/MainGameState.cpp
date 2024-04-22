// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/MainGameState.h"

#include "GameFramework/PlayerState.h"
#include "Gamemode/MainGameMode.h"
#include "Net/UnrealNetwork.h"



void AMainGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainGameState,CurrentWave);
	DOREPLIFETIME(AMainGameState,CurrentEnemies);
	DOREPLIFETIME(AMainGameState,TimeBeforeSpawnWaves);
	DOREPLIFETIME(AMainGameState,PlayerInfoArray);
}

void AMainGameState::EndGame()
{
	if (HasAuthority())
	{
		MulticastEndGame();
	}
}

void AMainGameState::MulticastEndGame_Implementation()
{
	bHasGameEnded = true;
	OnGameEnd.Broadcast(bHasGameEnded);
}

void AMainGameState::OnRep_CurrentWave()
{
	OnUpdatedCurrentWave.Broadcast(CurrentWave);
}

void AMainGameState::OnRep_CurrentEnemies()
{
	OnUpdatedEnemiesThisWave.Broadcast(CurrentEnemies);
}

void AMainGameState::OnRep_TimeBeforeSpawnWaves()
{
	OnSetTimeBeforeSpawnWaves.Broadcast(TimeBeforeSpawnWaves);
}

void AMainGameState::OnRep_PlayerInfoArray()
{
	OnPlayerInfoUpdated.Broadcast(PlayerInfoArray);
}

void AMainGameState::InitPlayerInfo(const int32& UniqueID,const FString& PlayerName)
{
	if (!PlayerInfoArray.IsEmpty())
	{
		for(const FPlayerInfo& Info : PlayerInfoArray)
		{
			if(Info.PlayerUniqueID == UniqueID)
			{
				return;
			}
		}
	}
	
	FPlayerInfo NewPlayerInfo;
	const FName PlayerFName = FName(*PlayerName);
	NewPlayerInfo.PlayerUniqueID = UniqueID;
	NewPlayerInfo.PlayerName = PlayerFName;
	PlayerInfoArray.Add(NewPlayerInfo);
	
	OnPlayerInfoUpdated.Broadcast(PlayerInfoArray);
}

void AMainGameState::SetTimeBeforeSpawnWaves(float Time)
{
	TimeBeforeSpawnWaves = Time;
	if(HasAuthority())
	{
		OnSetTimeBeforeSpawnWaves.Broadcast(TimeBeforeSpawnWaves);
	}
}

void AMainGameState::SetCurrentWave(int32 NewWave)
{
	CurrentWave = NewWave;
	if(HasAuthority())
	{
		OnUpdatedCurrentWave.Broadcast(CurrentWave);
	}
}

void AMainGameState::SetEnemiesThisWave(int32 Enemies)
{
	CurrentEnemies = Enemies;
	if(HasAuthority())
	{
		OnUpdatedEnemiesThisWave.Broadcast(CurrentEnemies);
	}
}

void AMainGameState::SetWhoKilled(const AActor* InstigatorActor)
{
	if(!InstigatorActor) return;
	
	const APlayerController* PlayerController = Cast<APlayerController>(InstigatorActor->GetInstigatorController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetWhoDead: DeadPlayer does not have a valid PlayerController."));
		return;
	}

	const APlayerState* PlayerState = PlayerController->GetPlayerState<APlayerState>();
	if (!PlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetWhoDead: PlayerController does not have a valid PlayerState."));
		return;
	}

	const FString PlayerName = PlayerState->GetPlayerName();

	for (FPlayerInfo& Info : PlayerInfoArray)
	{
		if (Info.PlayerName == PlayerName)
		{
			Info.EnemyKills++;
			break;
		}
	}

	OnPlayerInfoUpdated.Broadcast(PlayerInfoArray);
}

void AMainGameState::SetWhoDead(const AActor* DeadPlayer)
{
	if(!DeadPlayer) return;

	const APlayerController* PlayerController = Cast<APlayerController>(DeadPlayer->GetInstigatorController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetWhoDead: DeadPlayer does not have a valid PlayerController."));
		return;
	}

	const APlayerState* PlayerState = PlayerController->GetPlayerState<APlayerState>();
	if (!PlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetWhoDead: PlayerController does not have a valid PlayerState."));
		return;
	}

	const FString PlayerName = PlayerState->GetPlayerName();

	for (FPlayerInfo& Info : PlayerInfoArray)
	{
		if (Info.PlayerName == PlayerName)
		{
			Info.Deaths++;
			break;
		}
	}

	OnPlayerInfoUpdated.Broadcast(PlayerInfoArray);
}

