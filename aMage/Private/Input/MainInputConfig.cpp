// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/MainInputConfig.h"
#include "GameplayTagsSingleton.h"

const UInputAction* UMainInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for(const auto& [InputAction, ActionInputTag] : AbilitiesInputActions)
	{
		if(InputAction && ActionInputTag.MatchesTag(InputTag))
		{
			return InputAction;
		}
	}
	if(bLogNotFound)
	{
		UE_LOG(LogTemp,Error,TEXT("Can't Find AbilityInputAction for [%s],on InputConfig [%s]"),*InputTag.ToString(),*GetNameSafe(this));
	}
	return nullptr;
}
