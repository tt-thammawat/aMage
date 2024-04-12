// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MainAbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
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
	
	UFUNCTION(BlueprintCallable,Category = "MainAbilitySystemLibrary|GameplayMechanics")
	static bool IsPhysicalDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable,Category = "MainAbilitySystemLibrary|GameplayEffect")
	static void SetIsFireDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,bool bInIsFireDamage);
	
	UFUNCTION(BlueprintCallable,Category = "MainAbilitySystemLibrary|GameplayEffect")
	static void SetIsLightningDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,bool bInIsLightningDamage);
	
	UFUNCTION(BlueprintCallable,Category = "MainAbilitySystemLibrary|GameplayEffect")
	static void SetIsIceDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,bool bInIsIceDamage);
	
	UFUNCTION(BlueprintCallable,Category = "MainAbilitySystemLibrary|GameplayEffect")
	static void SetIsPhysicalDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,bool bInIsPhysicalDamage);

	//AI Damage Radius
	UFUNCTION(BlueprintCallable,Category = "MainAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,const TArray<AActor*>& ActorsToIgnore,float Radius,const FVector& SphereOrigin);

	//Revive Radius
	UFUNCTION(BlueprintCallable,Category = "MainAbilitySystemLibrary|GameplayMechanics")
	static void GetDeadPlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,const TArray<AActor*>& ActorsToIgnore,float Radius,const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable,Category = "MainAbilitySystemLibrary|GameplayMechanics")
	static void GetLiveActorsInBeam(const UObject* WorldContextObject,TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore,const FVector& BeamStart, const FVector& BeamEnd, const FVector& BeamSize);
	
	UFUNCTION(BlueprintPure,Category = "MainAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(AActor* FirstActor,AActor* SecondActor);
	
	//Gen Unique Key
	UFUNCTION(BlueprintCallable,Category = "MainAbilitySystemLibrary|GenerateUniqueKey")
	static FString GenerateUniqueKeyFromFName(FName Name);
};

