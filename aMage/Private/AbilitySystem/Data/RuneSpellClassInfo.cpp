// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/RuneSpellClassInfo.h"

TSubclassOf<UGameplayAbility> URuneSpellClassInfo::GetRuneSpellMatchingAbility(const TArray<FGameplayTag>& ReceivedRuneSpellTags)
{
    for (const auto& RuneSpell : AllRuneAbilities)
    {
        if (RuneSpell.RuneSpellTags.Num() == ReceivedRuneSpellTags.Num())
        {
            bool bAllTagsMatch = true;

            for (int32 i = 0; i < RuneSpell.RuneSpellTags.Num(); ++i)
            {
                if (RuneSpell.RuneSpellTags[i] != ReceivedRuneSpellTags[i])
                {
                    bAllTagsMatch = false;
                    break;
                }
            }
            if (bAllTagsMatch)
            {
                return RuneSpell.RuneSpellAbility;
            }
        }
    }
    return nullptr;
}

TSubclassOf<UGameplayAbility> URuneSpellClassInfo::GetStaffSpellMatchingAbility(
    const TArray<FGameplayTag>& ReceivedRuneSpellTags)
{
    for (const auto& RuneSpell : StaffAbilities)
    {
        if (RuneSpell.RuneSpellTags.Num() == ReceivedRuneSpellTags.Num())
        {
            bool bAllTagsMatch = true;

            for (int32 i = 0; i < RuneSpell.RuneSpellTags.Num(); ++i)
            {
                if (RuneSpell.RuneSpellTags[i] != ReceivedRuneSpellTags[i])
                {
                    bAllTagsMatch = false;
                    break;
                }
            }
            if (bAllTagsMatch)
            {
                return RuneSpell.RuneSpellAbility;
            }
        }
    }
    return nullptr;
}
