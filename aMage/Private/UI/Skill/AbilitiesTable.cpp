// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Skill/AbilitiesTable.h"

void UAbilitiesTable::InitializeAbilitiesToTMap(const TArray<FAbilitiesTagList>& AbilitiesTagLists)
{
	AllGroups.Empty();

	for (const FAbilitiesTagList& AbilityTagList : AbilitiesTagLists)
	{
		if (!AbilityTagList.Tags.IsEmpty())
		{
			FGameplayTag FirstTag = AbilityTagList.Tags[0];

			FAbilitiesTagListGroup* FoundGroup = AllGroups.FindByPredicate
			(
				[&](const FAbilitiesTagListGroup& Group)
				{
					return Group.GroupGameplayTag == FirstTag;
				}
			);

			if (FoundGroup)
			{
				FoundGroup->AbilitiesTagLists.Add(AbilityTagList);
			}
			else
			{
				FAbilitiesTagListGroup NewGroup;
				NewGroup.GroupGameplayTag = FirstTag;
				NewGroup.AbilitiesTagLists.Add(AbilityTagList);
				AllGroups.Add(NewGroup);
			}
		}
	}
	TableTagMapSet();
}