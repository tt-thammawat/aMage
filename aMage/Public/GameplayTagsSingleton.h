// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
*  MainGamePlayTags
 *  SingleTon Containing Native Gameplay Tags
 */
struct FMainGameplayTags
{
public:
 static const FMainGameplayTags& Get() {return MainGameplayTags;};
 static void InitializeNativeGameplayTags();

 //Declare Gameplay Tags For Primary Attributes
 FGameplayTag Attributes_Primary_Strength; // Damage
 FGameplayTag Attributes_Primary_Intelligence; // Mana
 FGameplayTag Attributes_Primary_Resilience; // Resistance
 FGameplayTag Attributes_Primary_Vigor; // HP
 
 //Declare Gameplay Tags For Secondary Attributes
 FGameplayTag Attributes_Secondary_FireRes;
 FGameplayTag Attributes_Secondary_IceRes;
 FGameplayTag Attributes_Secondary_ElectricRes;
 FGameplayTag Attributes_Secondary_MaxHealth;
 FGameplayTag Attributes_Secondary_MaxMana;
 //Vital Attributes
 FGameplayTag Attributes_Vital_Health;
 FGameplayTag Attributes_Vital_Mana;

 //Input Tag For Abilities Base Abilities And Casting
 FGameplayTag InputTag_LMB; 
 FGameplayTag InputTag_RMB;
 
 //Input Tag For Special Abilities
 FGameplayTag Event_Input_LMB;
 FGameplayTag Event_Input_RMB;
 //When Currently Using Special Abilities
 FGameplayTag Event_Actions_SpecialAbility;

private:
 static FMainGameplayTags MainGameplayTags;
};
