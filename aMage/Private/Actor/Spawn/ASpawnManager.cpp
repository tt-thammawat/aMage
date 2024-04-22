// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Spawn/ASpawnManager.h"

#include "Actor/InteractActor/MainItemInteractActor.h"
#include "Character/BaseEnemy.h"
#include "Gamemode/MainGameMode.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AASpawnManager::AASpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;
	
}

AASpawnManager::~AASpawnManager()
{
}

void AASpawnManager::BeginPlay()
{
	Super::BeginPlay();
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
	if(!HasAuthority()) return;
	
	if (TSubclassOf<ABaseEnemy>* EnemyClass = EnemyToSpawnClass.Find(EnemyType))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		GetWorld()->SpawnActor<ABaseEnemy>(*EnemyClass, SpawnLocation, SpawnRotation,SpawnParams);
	}
}

void AASpawnManager::DeleteAllChest_Implementation()
{
	if(!HasAuthority()) return;

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
	
	if (CurrentWaves >= 1 && CurrentWaves < 2)
	{
		SpawnableTypes.Add(ECharacterClass::Warrior);
		SpawnableTypes.Add(ECharacterClass::Ranger);
	}
	else if (CurrentWaves >= 2)
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
	if(!HasAuthority()) return;
	if(!ChestClass) return;
	
	TArray<FTransform> TempChestSpawnPoints = ChestsSpawnPointss;

	int32 BaseChestsToSpawn = FMath::Clamp((CurrentWaves * Difficulty) / 1.2, 1, ChestsSpawnPointss.Num());
	int32 MaxChestsCanSpawn = TempChestSpawnPoints.Num();
	int32 ChestsToSpawn = FMath::Min(BaseChestsToSpawn, MaxChestsCanSpawn);
    
	for (int32 i = 0; i < ChestsToSpawn; ++i)
	{
		if (TempChestSpawnPoints.Num() > 0)
		{
			const int32 PointIndex = FMath::RandRange(0, TempChestSpawnPoints.Num() - 1);

			FVector SpawnLocation = UKismetMathLibrary::TransformLocation(GetActorTransform(), TempChestSpawnPoints[PointIndex].GetLocation());
			FRotator SpawnRotation = UKismetMathLibrary::TransformRotation(GetActorTransform(), TempChestSpawnPoints[PointIndex].GetRotation().Rotator());

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            
			AActor* ChestActor = GetWorld()->SpawnActor<AMainInteractActor>(ChestClass, SpawnLocation, SpawnRotation, SpawnParams);
            
			SpawnChestArray.Add(ChestActor);
            
			TempChestSpawnPoints.RemoveAtSwap(PointIndex, 1, false);
		}
	}
}

void AASpawnManager::SpawnStaffs_Implementation(int32 CurrentWaves, int32 Difficulty)
{
	if (!HasAuthority() || CurrentWaves <= 1) return;
    if(StaffClassArray.IsEmpty()) return;
	
	TArray<FTransform> TempStaffSpawnPoints = StaffSpawnPoints;

	int32 BaseStaffToSpawn = FMath::Clamp((CurrentWaves * Difficulty) / 2, 0, StaffSpawnPoints.Num());
	
	int32 MaxStaffCanSpawn = TempStaffSpawnPoints.Num();
	int32 StaffToSpawn = FMath::Min(BaseStaffToSpawn, MaxStaffCanSpawn);
    
	for (int32 i = 0; i < StaffToSpawn; ++i)
	{
		if (TempStaffSpawnPoints.Num() > 0)
		{
			const int32 PointIndex = FMath::RandRange(0, TempStaffSpawnPoints.Num() - 1);
            
			FVector SpawnLocation = UKismetMathLibrary::TransformLocation(GetActorTransform(), TempStaffSpawnPoints[PointIndex].GetLocation());
			FRotator SpawnRotation = UKismetMathLibrary::TransformRotation(GetActorTransform(), TempStaffSpawnPoints[PointIndex].GetRotation().Rotator());

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            
			const int32 StaffIndex = FMath::RandRange(0, StaffClassArray.Num() - 1);
            
			AMainItemInteractActor* StaffActor = GetWorld()->SpawnActor<AMainItemInteractActor>(StaffClassArray[StaffIndex], SpawnLocation, SpawnRotation, SpawnParams);
			if (StaffActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			{
				IInteractInterface::Execute_HighlightActor(StaffActor);
			}
			
			SpawnStaffArray.Add(StaffActor);
            
			TempStaffSpawnPoints.RemoveAtSwap(PointIndex, 1, false);
		}
	}
}

void AASpawnManager::DeleteAllStaff_Implementation()
{
	if(!HasAuthority()) return;

	if(SpawnStaffArray.IsEmpty()) return;

	for(const TObjectPtr<AMainItemInteractActor>& Staff : SpawnStaffArray)
	{
		if(!Staff->GetIsPickup())
		{
			Staff->Destroy();
		}
	}

	SpawnStaffArray.Empty();
}

int32 AASpawnManager::GetCurrentEnemyCount_Implementation() const
{
	return ISpawnService::GetCurrentEnemyCount_Implementation();
}



