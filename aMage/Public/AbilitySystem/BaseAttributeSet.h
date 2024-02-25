// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BaseAttributeSet.generated.h"

//Define ACCESSOR FOR FGameplayAttributeData
// Now All Attributes can use Getter , setter , init without create new get/setter function
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
//Struct That Hold Info Of Both Source And Target
USTRUCT(BlueprintType)
struct FEffectProperties
{
	
	GENERATED_BODY()
	// Define The Constructor

	FEffectProperties() {}
	FEffectProperties(UAbilitySystemComponent* SourceASC,AActor* SourceAvatarActor,AController* SourceController,ACharacter* SourceCharacter,FGameplayEffectContextHandle EffectContextHandle) {}
	// End The Constructor

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AActor* SourceAvatarActor = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AController* SourceController= nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ACharacter* SourceCharacter= nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AController* TargetController= nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ACharacter* TargetCharacter= nullptr;
		
};

// template for modern c++ need to specifier Template manually but more Versatile
// TStaticFunctionPtr<FGameplayAttribute()>
// TStaticFunctionPtr<float(int32,float,int32)> RandomFunc;
// static float Random(int32 x,float y, int32 c);
// RandomFunc = Random;
// float result = RandomFunc(5,6.2f,8);
//Generic to any signature
template<class T>
using TStaticFunctionPtr = typename TBaseStaticDelegateInstance<T,FDefaultDelegateUserPolicy>::FFuncPtr;

//Can Use In Blueprint
UCLASS(BlueprintType)
class AMAGE_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UBaseAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	//Map Tags To GetAttribute Function
	TMap<FGameplayTag,TStaticFunctionPtr<FGameplayAttribute()>> TagsToAttributes;

	
	// Primary Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength,Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,Strength);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence,Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,Intelligence);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience,Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,Resilience);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor,Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,Vigor);
	
	// Secondary Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana,Category = "Secondary  Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,MaxMana);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth,Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,MaxHealth);
	
	//Resistance Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceFire,Category = "Secondary Attributes")
	FGameplayAttributeData ResistanceFire;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,ResistanceFire);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceLightning,Category = "Secondary Attributes")
	FGameplayAttributeData ResistanceLightning;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,ResistanceLightning);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceIce,Category = "Secondary Attributes")
	FGameplayAttributeData ResistanceIce;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,ResistanceIce);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistancePhysical,Category = "Secondary Attributes")
	FGameplayAttributeData ResistancePhysical;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,ResistancePhysical);
	
	// Vital Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana,Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,Mana);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health,Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,Health);

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_ResistanceFire(const FGameplayAttributeData& OldResistanceFire) const;
	UFUNCTION()
	void OnRep_ResistanceLightning(const FGameplayAttributeData& OldMResistanceLightning) const;
	UFUNCTION()
	void OnRep_ResistanceIce(const FGameplayAttributeData& OldResistanceIce) const;
	UFUNCTION()
	void OnRep_ResistancePhysical(const FGameplayAttributeData& OldResistancePhysical) const;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

/*
 *Meta Attributes
*/
	//Set On Server Process On Server
	UPROPERTY(BlueprintReadOnly,Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,IncomingDamage);
private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data,FEffectProperties& Props) const;
	void ShowFloatingText(const FEffectProperties& Props,float Damage,bool bIsFireDamage,bool bIsLightningDamage,bool bIsIceDamage,bool bIsPhysicDamage) const;
};
