// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CastingInterface.generated.h"

struct FGameplayTag;
// This class does not need to be modified.
UINTERFACE(MinimalAPI,Blueprintable)
class UCastingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AMAGE_API ICastingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void MatchRuneSpellTags(TArray<FGameplayTag> RuneTags)=0;
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
    void ClearRuneSpell();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void ReloadRuneSpell();
};
