// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UnistrokeResult.generated.h"

/**
 * 
 */
USTRUCT()
struct AMAGE_API FUnistrokeResult
{
	GENERATED_USTRUCT_BODY()

public:
	FUnistrokeResult();
	FUnistrokeResult(const FString &Name,const FGameplayTag& NameTag, const float &Score, const float &Time);
	~FUnistrokeResult();

public:
	FString Name;
	FGameplayTag NameTag;
	float Score;
	float Time;
};
