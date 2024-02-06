// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "MainInputAction.generated.h"

/**
 * 
 */
UCLASS()
class AMAGE_API UMainInputAction : public UInputAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Gameplay ABility System")
	FGameplayTag InputTag = FGameplayTag();
	
};
