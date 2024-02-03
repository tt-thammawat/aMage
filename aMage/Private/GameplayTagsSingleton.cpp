// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTagsSingleton.h"
#include "GameplayTagsManager.h"


FMainGameplayTags FMainGameplayTags::MainGameplayTags;

void FMainGameplayTags::InitializeNativeGameplayTags()
{
	//Primary Attributes--------------------------------------------------------------------------------------------------------//
	MainGameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"),FString("Increase Physical Damage"));
	MainGameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"),FString("Increase Mana, MaxMana And Magical Damage"));
	MainGameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"),FString("Increase Elemental Resistance"));
	MainGameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"),FString("Increase Health And MaxHealth"));
	//-------------------------------------------------------------------------------------------------------------//
	//Secondary Attributes--------------------------------------------------------------------------------------------------------//
	MainGameplayTags.Attributes_Secondary_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Fire"),FString("Increase Fire Resistance"));
	MainGameplayTags.Attributes_Secondary_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Ice"),FString("Increase Ice Resistance"));
	MainGameplayTags.Attributes_Secondary_Electric = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Electric"),FString("Increase Electric Resistance"));
	MainGameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"),FString("Increase Max Health"));
	MainGameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"),FString("Increase Max Mana"));
	//-------------------------------------------------------------------------------------------------------------//
	//Vital Attributes--------------------------------------------------------------------------------------------------------//
	MainGameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"),FString("Current Health"));
	MainGameplayTags.Attributes_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Mana"),FString("Current Mana"));
	//-------------------------------------------------------------------------------------------------------------//
	//InputTags----------------------------------------------------------------------------------------------------//
	MainGameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("Input Tag For Left Mouse Button"));
	MainGameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("Input Tag For Right Mouse Button"));
	MainGameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), FString("Input Tag For 1 Key"));
	MainGameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), FString("Input Tag For 2 Key"));
	//-------------------------------------------------------------------------------------------------------------//

	
}
