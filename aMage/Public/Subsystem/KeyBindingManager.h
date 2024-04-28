// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "KeyBindingManager.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNewKeySelectedSignature,const FName&,MappingName,const FInputChord&,InputKey);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyButtonPressedSignature,const FName&,MappingName);

UCLASS(BlueprintType)
class AMAGE_API UKeyBindingManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnNewKeySelectedSignature OnNewKeySelected;
	
	UPROPERTY(BlueprintAssignable)
	FOnKeyButtonPressedSignature OnKeyButtonPressed;
	
	UFUNCTION(BlueprintCallable)
	void OnKeySelected(const FName& MappingName,const FInputChord& InputKey);

	UFUNCTION(BlueprintCallable)
	void OnKeyPressed(const FName& MappingName);
};
