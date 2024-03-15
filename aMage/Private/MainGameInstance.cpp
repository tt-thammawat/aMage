// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"

UTexture2D* UMainGameInstance::GetSpellIconTexture(FName SpellName)
{
	UTexture2D** Texture = SpellTextures.Find(SpellName);
	return Texture ? *Texture : nullptr;
}

FText UMainGameInstance::GetSpellDescription(FName DescriptionName)
{
	FText* Text = SpellDescriptions.Find(DescriptionName);
	return Text ? *Text : FText::GetEmpty();
}
