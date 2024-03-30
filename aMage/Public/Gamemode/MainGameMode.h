// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Data/BaseRuneDataAsset.h"
#include "GameFramework/GameMode.h"
#include "MainGameMode.generated.h"

class AASpawnManager;
class AMainGameState;
class AMainInteractActor;
class ABaseEnemy;
class URuneSpellClassInfo;
class UCharacterClassInfo;



USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Info")
	int32 PlayerUniqueID=0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Info")
	FName PlayerName=FName();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Info")
	int32 EnemyKills = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Info")
	int32 Deaths = 0;
};

/**
 * 
 */
UCLASS()
class AMAGE_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AMainGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	
	int32 GetCurrentPlayerCount() const;
	
	//Start Wave-Logic
	UFUNCTION(BlueprintCallable)
	void StartNewWave();
	
	UFUNCTION()
	void WaitBeforeStartNewWaves(bool bIsFirstTime = false);
	
	void SpawnChest();
	
	UFUNCTION(BlueprintCallable)
	void EnemyKilled(AActor* InstigatorActor);

public:
	int32 GetCurrentWave() const { return CurrentWave; }
	void SetCurrentWave(int32 NewWave) { CurrentWave = NewWave; }
	
	//End Wave Logic
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<FPlayerInfo> PlayerInfoArray;
	
protected:
	virtual void BeginPlay() override;
private:
	TObjectPtr<AASpawnManager> SpawnManager;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true),Category = "Wave Management")
	TObjectPtr<AMainGameState> GS;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true),Category = "Wave Management")
	int32 CurrentWave = 0;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess=true),Category = "Wave Management")
	int32 BaseDifficulty = 1;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true),Category = "Wave Management")
	int32 BaseEnemiesPerWave = 2;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true),Category = "Wave Management")
	int32 CurrentEnemies = 0;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true),Category = "Wave Management")
	int32 WaveIncrement = 1;

	FTimerHandle TimerHandle_WaitBeforeStartWaves;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true),Category = "Wave Management")
	float TimeBeforeSpawnWaves=15.f;

	
public:
	UPROPERTY(EditDefaultsOnly,Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassDefaultInfo;
	
	//use ERuneType to get FBaseRuneTypeInfo from DataAsset
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Rune")
	TObjectPtr<UBaseRuneDataAsset> BaseRuneTypeInfo;

	//Item Rarity
	UFUNCTION(BlueprintCallable,Category ="Generate Rune")
	EItemRarity GenerateItemRarity();

	//Random Generate Base Rune Type
	UFUNCTION(BlueprintCallable,Category ="Generate Rune")
	ERuneType RandomRuneType();

	//Matching RuneSpell
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Rune Spell")
	TObjectPtr<URuneSpellClassInfo> RuneSpellClassInfos;

};
