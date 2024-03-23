// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,Blueprintable)
class UTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AMAGE_API ITargetInterface
{
	GENERATED_BODY()

public:

	// For Enemy
	// Pure Virtual Function Need Other class Need To override
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void HighlightActor();
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void UnHighlightActor();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	AActor* GetCombatTarget() const;
};
