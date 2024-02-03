// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnistrokeRectangle.generated.h"

/**
 * 
 */
USTRUCT()
struct AMAGE_API FUnistrokeRectangle
{
	GENERATED_USTRUCT_BODY()

public:
	FUnistrokeRectangle();
	FUnistrokeRectangle(const float& X, const float& Y, const float& Width, const float& Height);
	~FUnistrokeRectangle();

	public:
	float X;
	float Y;
	float Width;
	float Height;
};