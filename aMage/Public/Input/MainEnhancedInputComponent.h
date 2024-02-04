// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "MainInputAction.h"
#include "MainEnhancedInputComponent.generated.h"


class UMainInputAction;
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
	void BindAbilityActions(const UInputMappingContext* InputConfig,UserClass* Object,PressedFunctionType PressedFunction,ReleasedFunction ReleasedFunc,HeldFunction HeldFunc);
};

template <class UserClass, typename PressedFunctionType, typename ReleasedFunction, typename HeldFunction>
void UMainEnhancedInputComponent::BindAbilityActions(const UInputMappingContext* InputConfig, UserClass* Object,
	PressedFunctionType PressedFunction, ReleasedFunction ReleasedFunc, HeldFunction HeldFunc)
{
	check(InputConfig);
	for(const FEnhancedActionKeyMapping& Mapping : InputConfig->GetMappings())
	{
		if(UMainInputAction* MainInputAction = Cast<UMainInputAction>(Mapping.Action))
		{
		if(	MainInputAction->InputTag.IsValid())
		
			{
					// BindAction this Function already Inside UEnhancedInputComponent
					BindAction(MainInputAction,ETriggerEvent::Started,Object,PressedFunction,MainInputAction->InputTag);
					BindAction(MainInputAction,ETriggerEvent::Completed,Object,ReleasedFunc,MainInputAction->InputTag);
					BindAction(MainInputAction,ETriggerEvent::Triggered,Object,HeldFunc,MainInputAction->InputTag);
			}
		}
	}
}
