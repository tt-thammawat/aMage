// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interact/ICombatInterface.h"
#include "BaseCharacter.generated.h"

class UGameplayEffect;
class UGameplayAbility;
class UAbilitySystemComponent;
class UAttributeSet;
UCLASS()
class AMAGE_API ABaseCharacter : public ACharacter,public IAbilitySystemInterface,public  IICombatInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;};

	//Client Get This Info
	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastHandleDeath();
	
	//CombatInterface
	//Call From Server
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	//GetWeaponTipSocketName And Make FVector out of it
	virtual FVector GetCombatSocketLocation_Implementation() override;
	//EndCombatInterface
	
protected:
	virtual void BeginPlay() override;
	
	//Weapon
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	//Socket For Firing
	UPROPERTY(EditAnywhere,Category="Combat")
	FName WeaponTipSocketName;
	
	bool bDead = false;
	/*Dissolve Effects*/
	void Dissolve();
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* MaterialInstanceDynamic);
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* MaterialInstanceDynamic);
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();

	//Initialize Main Attributes
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = Attributes)
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	//Initialize Secondary Attributes
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = Attributes)
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	//Initialize Vital Attributes
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = Attributes)
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	//Apply Attributes To Self
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass,float Level) const;

	//Initialize DefaultAttributes
	virtual void InitDefaultAttributes() const;

	//Give Character Abilities
	void AddCharacterAbilities();
public:	

	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;
};
