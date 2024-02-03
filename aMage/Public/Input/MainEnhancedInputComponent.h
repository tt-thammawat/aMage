// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "MainInputConfig.h"
#include "MainEnhancedInputComponent.generated.h"

struct FMainInputAction;
class UMainInputConfig;
/**
 * 
 */
UCLASS()
class AMAGE_API UMainEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	//Make The Template For The Delegate
	template<class UserClass,typename PressedFunctionType,typename ReleasedFunctionType,typename HeldFunctionType>
	void BindAbilityAction(const UMainInputConfig* InputConfig,UserClass* Object,PressedFunctionType PressedFunction,ReleasedFunctionType ReleasedFunction,HeldFunctionType HeldFunction);

};

template <class UserClass, typename PressedFunctionType, typename ReleasedFunctionType, typename HeldFunctionType>
void UMainEnhancedInputComponent::BindAbilityAction(const UMainInputConfig* InputConfig, UserClass* Object,
	PressedFunctionType PressedFunction, ReleasedFunctionType ReleasedFunction,HeldFunctionType HeldFunction)
{
	check(InputConfig);
	//Break Struck That of InputConfig To Get Tag Then Bind That Action With InputAction
	for(const FMainInputAction& ActionInputs : InputConfig->AbilitiesInputActions)
	{
		if(PressedFunction)
		{
			//If it is PressedFunction(FGameplayTag x ); it can accept a single parameter of ActionInputs.ActionInputTag It have the same type
			BindAction(ActionInputs.InputAction,ETriggerEvent::Started,Object,PressedFunction,ActionInputs.ActionInputTag);
		}

		if(ReleasedFunction)
		{
			BindAction(ActionInputs.InputAction,ETriggerEvent::Completed,Object,ReleasedFunction,ActionInputs.ActionInputTag);
		}

		if(HeldFunction)
		{
			BindAction(ActionInputs.InputAction,ETriggerEvent::Triggered,Object,HeldFunction,ActionInputs.ActionInputTag);
		}
	}
}
