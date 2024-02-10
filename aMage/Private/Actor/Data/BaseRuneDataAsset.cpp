// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Data/BaseRuneDataAsset.h"


EItemRarity UBaseRuneDataAsset::GenerateItemRarity()
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

ERuneType UBaseRuneDataAsset::RandomRuneType()
{
	// Subtract 1 because we're using 'Max' as a non-selectable value
	int32 MaxEnumValue = static_cast<int32>(ERuneType::Max) - 1;

	// Generate a random index within the range of your actual enum values
	int32 RandomIndex = FMath::RandRange(0, MaxEnumValue);

	// Cast the random index back to your enum type
	ERuneType RandomRuneType = static_cast<ERuneType>(RandomIndex);

	return RandomRuneType;
}

FBaseItemTypeInfo UBaseRuneDataAsset::GetClassDefaultInfo(ERuneType RuneType)
{
	const FBaseItemTypeInfoList BaseRuneTypeInfoList = RuneTypeInformation.FindChecked(RuneType);
	if(BaseRuneTypeInfoList.ItemInfo.Num())
	{
		int32 RandomIndex = FMath::RandRange(0, BaseRuneTypeInfoList.ItemInfo.Num() - 1);
		return BaseRuneTypeInfoList.ItemInfo[RandomIndex];
	}
	else
	{
		return FBaseItemTypeInfo();
	}
}

FRuneSpecialAbility UBaseRuneDataAsset::GetRuneAbilitiesInfo(ERuneType RuneType)
{
	const FRuneSpecialAbilitiesList RuneSpecialAbilitiesList = RuneSpecialAbilitiesInformation.FindChecked(RuneType);
	if(RuneSpecialAbilitiesList.RuneCastingAbilitiesInfo.Num())
	{
		int32 RandomIndex = FMath::RandRange(0, RuneSpecialAbilitiesList.RuneCastingAbilitiesInfo.Num() - 1);
		return RuneSpecialAbilitiesList.RuneCastingAbilitiesInfo[RandomIndex];
	}
	else
	{
		return FRuneSpecialAbility();
	}
}
