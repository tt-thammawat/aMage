// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MainAbilitySystemGlobals.h"

#include "MainAbilityTypes.h"

FGameplayEffectContext* UMainAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FMainGameplayEffectContext();
}
