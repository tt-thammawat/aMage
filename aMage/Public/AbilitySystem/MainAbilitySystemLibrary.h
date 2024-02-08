// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MainAbilitySystemLibrary.generated.h"

enum class ECharacterClass : uint8;
class UAbilitySystemComponent;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AMAGE_API UMainAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//return OverlayWidgetController
	UFUNCTION(BlueprintPure,Category = "MainAbilitySystemLibrary|OverlayController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

//	UFUNCTION(BlueprintPure,Category = "MainAbilitySystemLibrary|MainWidgetController")
//	static  UAttributeMenuWidgetController* GetAttributeWidgetController(const UObject* WorldContextObject);
	
	//Get CharacterInfoData From GameMode
	UFUNCTION(BlueprintCallable,Category = "MainAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject,ECharacterClass CharacterClass,float Level,UAbilitySystemComponent* TargetASC);
	

};

