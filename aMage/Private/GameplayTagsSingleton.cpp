// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTagsSingleton.h"
#include "GameplayTagsManager.h"


FMainGameplayTags FMainGameplayTags::MainGameplayTags;

void FMainGameplayTags::InitializeNativeGameplayTags()
{
	//Primary Attributes---------------------------------------------------------------------------------------------//
	MainGameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"),FString("Increase Physical Damage"));
	MainGameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"),FString("Increase Mana, MaxMana And Magical Damage"));
	MainGameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"),FString("Increase Elemental Resistance"));
	MainGameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"),FString("Increase Health And MaxHealth"));
	//-------------------------------------------------------------------------------------------------------------//
	
	//Secondary Attributes------------------------------------------------------------------------------------------//
	MainGameplayTags.Attributes_Secondary_FireRes = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Fire"),FString("Increase Fire Resistance"));
	MainGameplayTags.Attributes_Secondary_IceRes = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Ice"),FString("Increase Ice Resistance"));
	MainGameplayTags.Attributes_Secondary_ElectricRes = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Electric"),FString("Increase Electric Resistance"));
	MainGameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"),FString("Increase Max Health"));
	MainGameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"),FString("Increase Max Mana"));
	//-------------------------------------------------------------------------------------------------------------//
	
	//Vital Attributes--------------------------------------------------------------------------------------------//
	MainGameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"),FString("Current Health"));
	MainGameplayTags.Attributes_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Mana"),FString("Current Mana"));
	//-------------------------------------------------------------------------------------------------------------//
	
	//InputTags----------------------------------------------------------------------------------------------------//
	MainGameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("Input Tag For Left Mouse Button"));
	MainGameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("Input Tag For Right Mouse Button"));
	//-------------------------------------------------------------------------------------------------------------//
	
	//Equipping Tag---------------------------------------------------------------------------------------------//
	MainGameplayTags.Item_Equip_Staff = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Item.Equip.Staff"),FString("Equip Item Staff Tags"));

	//-------------------------------------------------------------------------------------------------------------//
	
	//Special Abilities---------------------------------------------------------------------------------------------//

	//-------------------------------------------------------------------------------------------------------------//

	//State---------------------------------------------------------------------------------------------//
	MainGameplayTags.State_Action_Casting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Action.Casting"),FString("State When Casting"));
	MainGameplayTags.State_Action_Aiming = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Action.Aiming"),FString("State When Aiming"));
	MainGameplayTags.State_InAir_Jumping = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.InAir.Jumping"),FString("State When Jumping In The Air"));

	//-------------------------------------------------------------------------------------------------------------//

}
