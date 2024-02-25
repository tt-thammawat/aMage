// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

/**
 * 
 */
class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Wizard,
	Warrior,
	Ranger,
	Fodder
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()
	//UGameplayEffect Contains Tags and Effects of Attributes
	UPROPERTY(EditDefaultsOnly,Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly,Category = "Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
};

UCLASS()
class AMAGE_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;
	
	UPROPERTY(EditDefaultsOnly,Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	//Like Dead , Get Hit
	UPROPERTY(EditDefaultsOnly,Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;
	
	//TMap To Match Class And Attributes Struct
	UPROPERTY(EditDefaultsOnly,Category = "Class Defaults")
	TMap<ECharacterClass,FCharacterClassDefaultInfo> CharacterClassInformation;

	UPROPERTY(EditDefaultsOnly,Category = "Class Defaults|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;
	
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
};
