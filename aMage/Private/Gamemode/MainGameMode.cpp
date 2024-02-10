// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/MainGameMode.h"

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
