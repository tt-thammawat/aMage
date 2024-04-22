// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MainGameState.generated.h"

/**
 * 
 */


struct FPlayerInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdatedCurrentWaveSignature, int32, CurrentWave);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdatedEnemiesThisWaveSignature,int32,CurrentEnemies);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetTimeBeforeSpawnWavesSignature,float,TimeBeforeSpawnWaves);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerInfoUpdatedSignature,const TArray<FPlayerInfo>&,PlayerInfoArray);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEndSignature, bool, bIsEndGame);


UCLASS()
class AMAGE_API AMainGameState : public AGameState
{
	GENERATED_BODY()
public:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable)
	FOnUpdatedCurrentWaveSignature OnUpdatedCurrentWave;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdatedEnemiesThisWaveSignature OnUpdatedEnemiesThisWave;
	
	UPROPERTY(BlueprintAssignable)
	FOnSetTimeBeforeSpawnWavesSignature OnSetTimeBeforeSpawnWaves;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerInfoUpdatedSignature OnPlayerInfoUpdated;
	
	UPROPERTY(BlueprintAssignable)
	FOnGameEndSignature OnGameEnd;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game")
	bool bHasGameEnded = false;

	void EndGame();
	UFUNCTION(NetMulticast,Reliable)
	void MulticastEndGame();
	UFUNCTION()
	void OnRep_CurrentWave();
	UFUNCTION()
	void OnRep_CurrentEnemies();
	UFUNCTION()
	void OnRep_TimeBeforeSpawnWaves();
	UFUNCTION()
	void OnRep_PlayerInfoArray();
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, ReplicatedUsing=OnRep_PlayerInfoArray,meta=(AllowPrivateAccess=true))
	TArray<FPlayerInfo> PlayerInfoArray;
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentWave)
	int32 CurrentWave = 0;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentEnemies)
	int32 CurrentEnemies = 0;

	UPROPERTY(ReplicatedUsing=OnRep_TimeBeforeSpawnWaves)
	float TimeBeforeSpawnWaves;
	
public:
	UFUNCTION()
	void InitPlayerInfo(const int32& UniqueID,const FString& PlayerName);
	
	UFUNCTION()
	void SetTimeBeforeSpawnWaves(float Time);
	
	UFUNCTION()
	void SetCurrentWave(int32 NewWave);

	UFUNCTION()
	void SetEnemiesThisWave(int32 Enemies);

	UFUNCTION()
	void SetWhoKilled(const AActor* InstigatorActor);

	UFUNCTION()
	void SetWhoDead(const AActor* DeadPlayer);
	
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentWave() const { return CurrentWave; }

	UFUNCTION(BlueprintCallable)
	TArray<FPlayerInfo> GetPlayerInfoArray() const { return PlayerInfoArray; }
};