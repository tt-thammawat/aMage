// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Data/BaseRuneDataAsset.h"



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
