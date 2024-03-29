// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/MainGameState.h"

#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"



void AMainGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainGameState,CurrentWave);
	DOREPLIFETIME(AMainGameState,CurrentEnemies);
	DOREPLIFETIME(AMainGameState,TimeBeforeSpawnWaves);
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
	const FName PlayerFName = FName(*PlayerName);

	for(const FPlayerInfo& Info : PlayerInfoArray)
	{
		if(Info.PlayerUniqueID == UniqueID)
		{
			return;
		}
	}
	
	FPlayerInfo NewPlayerInfo;
	NewPlayerInfo.PlayerUniqueID = UniqueID;
	NewPlayerInfo.PlayerName = PlayerFName;
	PlayerInfoArray.Add(NewPlayerInfo);
	
	if(HasAuthority())
	{
		OnPlayerInfoUpdated.Broadcast(PlayerInfoArray);
	}
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

void AMainGameState::SetWhoKilled(AActor* InstigatorActor)
{
	if(!InstigatorActor) return;
	
	const APlayerController* PlayerController = Cast<APlayerController>(InstigatorActor->GetInstigatorController());
	
	if(PlayerController)
	{
		if(const APlayerState* PlayerState = PlayerController->GetPlayerState<APlayerState>())
		{
			const FString PlayerName = PlayerState->GetPlayerName();

			for(FPlayerInfo& Info : PlayerInfoArray)
			{
				if(Info.PlayerName == PlayerName)
				{
					Info.EnemyKills++;
				}
			}

			if(HasAuthority())
			{
				OnPlayerInfoUpdated.Broadcast(PlayerInfoArray);
			}
		}
	}
}

