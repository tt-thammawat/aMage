// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UnistrokeDataTable.generated.h"

/**
 * 
 */
USTRUCT()
struct  AMAGE_API FUnistrokeDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FUnistrokeDataTable();
	~FUnistrokeDataTable();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector2D> Points;

};