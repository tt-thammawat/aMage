// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "ICombatInterface.generated.h"

USTRUCT(BlueprintType)
struct FTaggedMontage

{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class UAnimMontage* Montage = nullptr;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag MontageTag;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI,Blueprintable)
class UICombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *  
 */
class AMAGE_API IICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int32 GetCharacterLevel();
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);
	
	//BlueprintNativeEvent automatic generate virtual
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "HitReact")
	UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	TArray<FTaggedMontage> GetAttackMontage();
	
	// Dead Do
	virtual void Die()=0;

	//Check if actor Dead
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	bool IsDead() const;

	//For Multiple Target
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	AActor* GetAvatar();
	
	// Declare the function as a BlueprintImplementableEvent //FAcing Enemy
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Targeting")
	void UpdateFacingTarget(const FVector& TargetVector);
	
};
