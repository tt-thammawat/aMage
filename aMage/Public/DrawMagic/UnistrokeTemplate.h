// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawMagic/UnistrokePoint.h"
#include "UnistrokeTemplate.generated.h"

static const int NumPoints = 64;
static const float SquareSize = 250.0f;

/**
 * 
 */
USTRUCT()
struct AMAGE_API FUnistrokeTemplate
{
	GENERATED_USTRUCT_BODY()

public:
	FUnistrokeTemplate();
	FUnistrokeTemplate(const FString &Name, const TArray<FUnistrokePoint> &Points);
	~FUnistrokeTemplate();

public:
	FString Name;
	TArray<float> Vector;
	TArray<FUnistrokePoint> Points;
};
