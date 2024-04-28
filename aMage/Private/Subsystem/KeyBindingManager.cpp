// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/KeyBindingManager.h"

void UKeyBindingManager::OnKeySelected(const FName& MappingName, const FInputChord& InputKey)
{
	if (OnNewKeySelected.IsBound())
	{
		OnNewKeySelected.Broadcast(MappingName,InputKey);
	}
}

void UKeyBindingManager::OnKeyPressed(const FName& MappingName)
{
	if (OnKeyButtonPressed.IsBound())
	{
		OnKeyButtonPressed.Broadcast(MappingName);
	}
}

