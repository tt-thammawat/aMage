// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagsSingleton.h"
#include "Character/MainPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MainPlayerCharacter = Cast<AMainPlayerCharacter>(TryGetPawnOwner());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(MainPlayerCharacter == nullptr)
	{
		MainPlayerCharacter = Cast<AMainPlayerCharacter>(TryGetPawnOwner());
	}
	if(MainPlayerCharacter == nullptr) return;

	FVector Velocity = MainPlayerCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = MainPlayerCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = MainPlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bIsCrouch = MainPlayerCharacter->bIsCrouched;
	if(MainPlayerCharacter && MainPlayerCharacter->GetAbilitySystemComponent())
	{
		MainPlayerCharacter->GetAbilitySystemComponent()->GetOwnedGameplayTags(GameplayTagContainer);
		bIsEquippedStaff = GameplayTagContainer.HasTag(FMainGameplayTags::Get().Item_Equip_Staff);
		bIsCasting = GameplayTagContainer.HasTag(FMainGameplayTags::Get().State_Action_Casting);
		AO_Yaw = MainPlayerCharacter->GetAOYaw();
		AO_Pitch = MainPlayerCharacter->GetAOPitch();
		bAiming = MainPlayerCharacter->GetIsAiming();
	}
	
	//TODO: Implement AIming
	
}
