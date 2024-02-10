// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UICombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *  
 */
class AMAGE_API IICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int32 GetCharacterLevel();
	virtual FVector GetCombatSocketLocation();

	//TODO:MAY Make These AutoLock
	// Declare the function as a BlueprintImplementableEvent //FAcing Enemy
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Targeting")
	void UpdateFacingTarget(const FVector& TargetVector);
};
