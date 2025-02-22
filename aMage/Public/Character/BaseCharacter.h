// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interact/ICombatInterface.h"
#include "BaseCharacter.generated.h"

class UFootStepsComponent;
class UGameplayEffect;
class UGameplayAbility;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS()
class AMAGE_API ABaseCharacter : public ACharacter,public IAbilitySystemInterface,public  IICombatInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	
	//CombatInterface
	//Call From Server
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die(const AActor* InstigatorActor) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual USceneComponent* GetSceneComponent_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontage_Implementation() override;
	//GetWeaponTipSocketName And Make FVector out of it
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	//EndCombatInterface

	//Client Get This Info
	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastHandleDeath();
	
	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeath;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	TArray<FTaggedMontage> AttackMontage;
	UFootStepsComponent* GetFootStepsComponent() const {return FootStepsComponent;};
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UFootStepsComponent> FootStepsComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USceneComponent> TargetSceneComponent;
	
	UPROPERTY()
	FTimerHandle CapsuleTimerHandle;
	void UpdateCapsuleAndRecoverLocation();
	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastHandleUpdateCapsuleAndRecoverLocation();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UAnimMontage> RecoverAnimMontage;
	void OnRecoverMontageEnded(UAnimMontage* Montage, bool bInterrupted = false);

	//Weapon
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	//Socket For Firing
	UPROPERTY(EditAnywhere,Category="Combat")
	TMap<FGameplayTag,FName> MappedSocketName;

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
	
	bool GetIsDead() const {return bDead;};

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;};
};
