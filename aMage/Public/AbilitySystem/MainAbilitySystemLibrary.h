// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MainAbilitySystemLibrary.generated.h"

class UCharacterClassInfo;
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
	
	UFUNCTION(BlueprintCallable,Category = "MainAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	// Abilities
	UFUNCTION(BlueprintCallable,Category = "MainAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartUpAbilities(const UObject* WorldContextObject,UAbilitySystemComponent* TargetASC,ECharacterClass CharacterClass);
	
	//Damage
	UFUNCTION(BlueprintPure,Category = "MainAbilitySystemLibrary|GameplayEffect")
	static bool IsFireDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure,Category = "MainAbilitySystemLibrary|GameplayEffect")
	
	static bool IsLightningDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure,Category = "MainAbilitySystemLibrary|GameplayEffect")
	
	static bool IsIceDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure,Category = "MainAbilitySystemLibrary|GameplayEffect")
	
	static bool IsPhysicalDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable,Category = "MainAbilitySystemLibrary|GameplayMechanics")

	//Damage
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,const TArray<AActor*>& ActorsToIgnore,float Radius,const FVector& SphereOrigin);
	
	UFUNCTION(BlueprintCallable,Category = "MainAbilitySystemLibrary|CharacterClassDefaults")
	static FString GenerateUniqueKeyFromFName(FName Name);
};

