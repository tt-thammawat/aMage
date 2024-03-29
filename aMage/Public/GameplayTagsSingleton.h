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
 FGameplayTag Attributes_Secondary_MaxHealth;
 FGameplayTag Attributes_Secondary_MaxMana;
 FGameplayTag Attributes_Secondary_MaxMoveSpeed;
 //Vital Attributes
 FGameplayTag Attributes_Vital_Health;
 FGameplayTag Attributes_Vital_Mana;

 //Input Tag For Abilities Base Abilities And Casting
 FGameplayTag InputTag_LMB; 
 FGameplayTag InputTag_RMB;
 //Loose Input Tag // Not:Use Yet
 FGameplayTag Input_Loose_LMB;
 FGameplayTag Input_Loose_RMB;
 
 //Equipping Tag
 FGameplayTag Item_Equip_Staff;

 //State Tag
 FGameplayTag State_Action_Casting;
 FGameplayTag State_Action_Aiming;
 FGameplayTag State_InAir_Jumping;
 FGameplayTag State_Movement_Crouch;

 FGameplayTag State_Action_Spell_NormalSpell;

 //Resistance
 FGameplayTag Attributes_Resistance_Fire;
 FGameplayTag Attributes_Resistance_Lightning;
 FGameplayTag Attributes_Resistance_Ice;
 FGameplayTag Attributes_Resistance_Physical;

 //Damage
 FGameplayTag Damage;
 FGameplayTag Damage_Fire;
 FGameplayTag Damage_Lightning;
 FGameplayTag Damage_Ice;
 FGameplayTag Damage_Physical;
 TMap<FGameplayTag,FGameplayTag> DamageTypesToResistances;
 
 //Abilities
 FGameplayTag Ability_Action_Attack;
 FGameplayTag Ability_Rune_NormalSpell;
 FGameplayTag Ability_Buff_Potion;

 //Socket For Enemies
 FGameplayTag Montage_Attack_Weapon;
 FGameplayTag Montage_Attack_RightHand;
 FGameplayTag Montage_Attack_LeftHand;
 
 //Rune Drawing
 FGameplayTag Rune_Tag_04_Clear;

 FGameplayTag Effects_Buff_Death;
 FGameplayTag Effects_HitReact;
 
private:
 static FMainGameplayTags MainGameplayTags;
};
