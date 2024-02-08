// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Data/BaseRuneDataAsset.h"


FBaseItemTypeInfo UBaseRuneDataAsset::GetClassDefaultInfo(ERuneType RuneType)
{
	return BaseRuneTypeInformation.FindChecked(RuneType);
}
