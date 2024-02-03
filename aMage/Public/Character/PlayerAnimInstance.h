// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/TurnInPlace.h"
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
	UPROPERTY(BlueprintReadOnly)
	bool bAiming;

	//ISStaffEquipped
	UPROPERTY(BlueprintReadWrite)
	bool IsEquippedStaff;
	
	
	

};
