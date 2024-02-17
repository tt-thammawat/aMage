// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/RuneSpellClassInfo.h"

TSubclassOf<UGameplayAbility> URuneSpellClassInfo::GetRuneSpellMatchingAbility(
	const FGameplayTagContainer& RuneTagContainer)
{
	for (auto& RuneSpell : AllRuneAbilities)
	{
		if(RuneTagContainer.HasAllExact(RuneSpell.RuneSpellTags))
		{
			return RuneSpell.RuneSpellAbility;
		}
	}
	return nullptr;
}
