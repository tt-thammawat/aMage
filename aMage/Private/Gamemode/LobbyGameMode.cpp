// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	if(GameState)
	{
		int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
		if(NumberOfPlayers == 10)
		{
			UWorld* World=GetWorld();
			if(World)
			{
				bUseSeamlessTravel=true;
				World->ServerTravel(FString("/Game/TestingMap?listen"));
			}
		}
	}

}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}
