// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MainGenericGameplayAbility.h"
#include "MainRuneSpellGameplayAbility.generated.h"

class UMainInputAction;
/**
 * 
 */
UCLASS()
class AMAGE_API UMainRuneSpellGameplayAbility : public UMainGenericGameplayAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere,Category=Spell)
	float UsageTimes;
	
	UPROPERTY(EditAnywhere,Category=MatchRuneTag)
	TObjectPtr<UMainInputAction> LMBInputAction;
	UPROPERTY(EditAnywhere,Category=MatchRuneTag)
	TObjectPtr<UMainInputAction> RMBInputAction;

	
};
