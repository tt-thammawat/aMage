// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/RuneSpellClassInfo.h"

TSubclassOf<UGameplayAbility> URuneSpellClassInfo::GetRuneSpellMatchingAbility(const TArray<FGameplayTag>& ReceivedRuneSpellTags)
{
    for (const auto& RuneSpell : AllRuneAbilities)
    {
        // Check if the number of tags match
        if (RuneSpell.RuneSpellTags.Num() == ReceivedRuneSpellTags.Num())
        {
            bool bAllTagsMatch = true;

            // Check each tag for a match
            for (int32 i = 0; i < RuneSpell.RuneSpellTags.Num(); ++i)
            {
                if (RuneSpell.RuneSpellTags[i] != ReceivedRuneSpellTags[i])
                {
                    bAllTagsMatch = false;
                    break; // Break out of the loop as soon as one tag does not match
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
