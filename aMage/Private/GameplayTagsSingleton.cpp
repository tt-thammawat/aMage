// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTagsSingleton.h"
#include "GameplayTagsManager.h"


FMainGameplayTags FMainGameplayTags::MainGameplayTags;

void FMainGameplayTags::InitializeNativeGameplayTags()
{
	//Primary Attributes
	MainGameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"),FString("Increase Physical Damage"));
	MainGameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"),FString("Increase Mana, MaxMana And Magical Damage"));
	MainGameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"),FString("Increase Elemental Resistance"));
	MainGameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"),FString("Increase Health And MaxHealth"));
	MainGameplayTags.Attributes_Secondary_MaxMoveSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.MaxMoveSpeed"),FString("Increase Max MoveSpeed"));

	//Secondary Attributes
	MainGameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"),FString("Increase Max Health"));
	MainGameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"),FString("Increase Max Mana"));

	//Vital Attributes
	MainGameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"),FString("Current Health"));
	MainGameplayTags.Attributes_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Mana"),FString("Current Mana"));
	
	//Resistance Type
	MainGameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"), FString("Fire Resistance"));
	MainGameplayTags.Attributes_Resistance_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Ice"), FString("Ice Resistance"));
	MainGameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"), FString("Lightning Resistance"));
	MainGameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"), FString("Physical Resistance"));
	
	//Damage Types
	MainGameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage"));
	MainGameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire")),FString("Fire Damage Type");
	MainGameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"), FString("Lightning Damage Type"));
	MainGameplayTags.Damage_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Ice")),FString("Ice Damage Type");
	MainGameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical")),FString("Physical Damage Type");

	//Map Of Damage Types To Resistances
	MainGameplayTags.DamageTypesToResistances.Add(MainGameplayTags.Damage_Fire,MainGameplayTags.Attributes_Resistance_Fire);
	MainGameplayTags.DamageTypesToResistances.Add(MainGameplayTags.Damage_Lightning,MainGameplayTags.Attributes_Resistance_Lightning);
	MainGameplayTags.DamageTypesToResistances.Add(MainGameplayTags.Damage_Ice,MainGameplayTags.Attributes_Resistance_Ice);
	MainGameplayTags.DamageTypesToResistances.Add(MainGameplayTags.Damage_Physical,MainGameplayTags.Attributes_Resistance_Physical);
	
	//Equipping Tag
	MainGameplayTags.Item_Equip_Staff = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Item.Equip.Staff"),FString("Equip Item Staff Tags"));

	//InputTags
	MainGameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("Input Tag For Left Mouse Button"));
	MainGameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("Input Tag For Right Mouse Button"));

	//LooseInputTags
	MainGameplayTags.Input_Loose_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Loose.LMB"), FString("Loose Input Tag For Left Mouse Button"));
	MainGameplayTags.Input_Loose_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Loose.RMB"), FString("Loose Input Tag For Right Mouse Button"));
	
	//State
	MainGameplayTags.State_Action_Casting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Action.Casting"),FString("State When Casting"));
	MainGameplayTags.State_Action_Aiming = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Action.Aiming"),FString("State When Aiming"));
	MainGameplayTags.State_InAir_Jumping = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.InAir.Jumping"),FString("State When Jumping In The Air"));
	MainGameplayTags.State_Action_Spell_NormalSpell = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Action.Spell.NormalSpell"),FString("State When Activate Normal Spell"));
	MainGameplayTags.State_Movement_Crouch = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Movement.Crouch"),FString("State When Crouching"));
	
	//Rune Drawing
	MainGameplayTags.Rune_Tag_04_Clear = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Rune.Tag.04.Clear"), FString("Rune Tag Clear"));

	//Abilities
	MainGameplayTags.Ability_Action_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Action.Attack"), FString("Attack Ability Tag"));
	MainGameplayTags.Ability_Rune_NormalSpell = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Rune.NormalSpell"), FString("Rune NormalSpell Ability Tag"));
	MainGameplayTags.Ability_Buff_Potion = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Buff.Potion"), FString("Buff Potion Ability Tag"));

	//Effects
	MainGameplayTags.Effects_Buff_Death = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.Buff.Death"), FString("Death"));
	MainGameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("HitReact"));

	//Montage
	MainGameplayTags.Montage_Attack_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.Weapon"), FString("Weapon"));
	MainGameplayTags.Montage_Attack_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.LeftHand"), FString("LeftHand"));
	MainGameplayTags.Montage_Attack_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.RightHand"), FString("RightHand"));
	
}
