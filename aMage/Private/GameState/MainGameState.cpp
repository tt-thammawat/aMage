// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/MainGameState.h"


void AMainGameState::SpawnItemWithRarity()
{
	if (HasAuthority())
	{
		EItemRarity GeneratedRarity = GenerateItemRarity();
	}
}

EItemRarity AMainGameState::GenerateItemRarity()
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
