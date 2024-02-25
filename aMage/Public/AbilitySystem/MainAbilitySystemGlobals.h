// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "MainAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class AMAGE_API UMainAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	// Override EffectContext for other damage like crit , fire ,element
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
