// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interact/TargetInterface.h"
#include "GameFramework/PlayerController.h"
#include "Interact/CastingInterface.h"
#include "MainPlayerController.generated.h"

class UMainInputAction;
class UBaseAbilitySystemComponent;
struct FGameplayTag;
class UMainInputConfig;
class UCombatActorComponent;
/**
 * 
 */
DECLARE_DELEGATE(FOnInteractButtonPressedSignature);
class ITargetInterface;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

UCLASS()
class AMAGE_API AMainPlayerController : public APlayerController,public ICastingInterface
{
	GENERATED_BODY()
public:
	AMainPlayerController();
	FOnInteractButtonPressedSignature InteractButtonPressedSignature;
protected:
	
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void PostInitializeComponents() override;
	void MoveAction(const FInputActionValue& InputActionValue) ;
	void Turn(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void InteractButtonPressed();
	void CrouchButtonPressed();
	void CastButtonPressed(const FInputActionValue& Value);
	void JumpButtonPressed();
	void JumpingRelease();

	//TODO:: Change It For Widget
	//void CursorTrace();


private:
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> MainInputContext;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> MoveInput;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> JumpAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> LookXAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> LookYAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> InteractButton;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> CrouchAction;
	
	
	//Activate Abilities
	void AbilityInputTagPressed(const FGameplayTag InputTag);
	void AbilityInputTagReleased(const FGameplayTag InputTag);
	void AbilityInputTagHeld(const FGameplayTag InputTag);

	UPROPERTY()
	TObjectPtr<UBaseAbilitySystemComponent> BaseAbilitySystemComponent;
	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent();

	ITargetInterface* LastActor;
	ITargetInterface* ThisActor;
	
	/*************************************************************************************************************/
	//DRAW FUNCTION IS HERE!

private:
	bool bIsDrawingSpell;
	
	UFUNCTION()
	void SetIsCastingDrawingWidget_Implementation(bool bIsDrawing) override;

public:
	FORCEINLINE bool GetIsDrawingSpell() const {return bIsDrawingSpell;};

};


