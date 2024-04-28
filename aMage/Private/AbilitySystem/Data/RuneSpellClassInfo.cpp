// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/RuneSpellClassInfo.h"

void URuneSpellClassInfo::InitializeHashRuneAbilitiesMapping()
{
    for (const FRuneAbilityMapping& RuneSpell : AllRuneAbilities)
    {
        uint32 Hash = GetTypeHash(RuneSpell.RuneSpellTags);
        TagHashToRuneAbilityMap.Add(Hash, RuneSpell.RuneSpellAbility);
    }
}

void URuneSpellClassInfo::InitializeHashStaffAbilitiesMapping()
{
    for (const FRuneAbilityMapping& RuneSpell : StaffAbilities)
    {
        uint32 Hash = GetTypeHash(RuneSpell.RuneSpellTags);
        TagHashToStaffAbilityMap.Add(Hash, RuneSpell.RuneSpellAbility);
    }
}

TSubclassOf<UGameplayAbility> URuneSpellClassInfo::GetRuneSpellMatchingAbility(const TArray<FGameplayTag>& ReceivedRuneSpellTags)
{
    uint32 ReceivedHash = GetTypeHash(ReceivedRuneSpellTags);
    TSubclassOf<UGameplayAbility>* FoundAbility = TagHashToRuneAbilityMap.Find(ReceivedHash);
    if (FoundAbility)
    {
        return *FoundAbility;
    }
    return nullptr;
}

TSubclassOf<UGameplayAbility> URuneSpellClassInfo::GetStaffSpellMatchingAbility(
    const TArray<FGameplayTag>& ReceivedRuneSpellTags)
{
    {
        uint32 ReceivedHash = GetTypeHash(ReceivedRuneSpellTags);
        TSubclassOf<UGameplayAbility>* FoundAbility = TagHashToStaffAbilityMap.Find(ReceivedHash);
        if (FoundAbility)
        {
            return *FoundAbility;
        }
        return nullptr;
    }
}
