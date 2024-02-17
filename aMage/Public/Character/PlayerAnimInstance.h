// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class AMainPlayerCharacter;
/**
 * 
 */
UCLASS()
class AMAGE_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AMainPlayerCharacter> MainPlayerCharacter;
	UPROPERTY(BlueprintReadOnly)
	float Speed;
	UPROPERTY(BlueprintReadOnly)
	bool bIsInAir;
	UPROPERTY(BlueprintReadOnly)
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly)
	bool bIsCrouch;


	//For Check Tag
	FGameplayTagContainer GameplayTagContainer;

	//StaffEquipped?
	UPROPERTY(BlueprintReadOnly)
	bool bIsEquippedStaff;

	UPROPERTY(BlueprintReadOnly)
	bool bIsCasting;

	//Aiming to Shoot Spell
	UPROPERTY(BlueprintReadOnly)
	bool bAiming;
	UPROPERTY(BlueprintReadOnly)
	float AO_Yaw;
	UPROPERTY(BlueprintReadOnly)
	float AO_Pitch;
	

};
