// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"

UTexture2D* UMainGameInstance::GetItemIconTexture(FName IconId)
{
	UTexture2D** Texture = IconTextures.Find(IconId);
	return Texture ? *Texture : nullptr;
}

FText UMainGameInstance::GetItemDescription(FName DescriptionId)
{
	FText* Text = Descriptions.Find(DescriptionId);
	return Text ? *Text : FText::GetEmpty();
}
