// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "MainInputConfig.h"
#include "MainEnhancedInputComponent.generated.h"


/**
 * 
 */
UCLASS()
class AMAGE_API UMainEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	//Make This Template To Assign Which Action would be Pressed , Release , Held
	//Template class allow to set generic placeholder can pass any type,functions to this function
	//Can work with inputs or types that are not yet defined or that can vary. 
	template<class UserClass,typename PressedFunctionType, typename ReleasedFunction,typename HeldFunction>
	void BindAbilityActions(const UMainInputConfig* InputConfig,UserClass* Object,PressedFunctionType PressedFunction,ReleasedFunction ReleasedFunc,HeldFunction HeldFunc);
};

template <class UserClass, typename PressedFunctionType, typename ReleasedFunction, typename HeldFunction>
void UMainEnhancedInputComponent::BindAbilityActions(const UMainInputConfig* InputConfig, UserClass* Object,
	PressedFunctionType PressedFunction, ReleasedFunction ReleasedFunc, HeldFunction HeldFunc)
{
	check(InputConfig);
	for(const FGMainInputAction& Actions : InputConfig->AbilitiesInputActions)
	{
		if(Actions.InputAction && Actions.ActionInputTag.IsValid())
		{
			if(PressedFunction)
			{
				// BindAction this Function already Inside UEnhancedInputComponent
				BindAction(Actions.InputAction,ETriggerEvent::Started,Object,PressedFunction,Actions.ActionInputTag);
			}

			if(ReleasedFunc)
			{
				BindAction(Actions.InputAction,ETriggerEvent::Completed,Object,ReleasedFunc,Actions.ActionInputTag);
			}
			
			if(HeldFunc)
			{
				BindAction(Actions.InputAction,ETriggerEvent::Triggered,Object,HeldFunc,Actions.ActionInputTag);
			}
		}
	}
}

