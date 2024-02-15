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
/**
 * 
 */
DECLARE_DELEGATE(FOnInteractButtonPressedSignature);
DECLARE_DELEGATE_OneParam(FOnButtonPressedSignature,int);
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
	FOnButtonPressedSignature OnButtonPressed;

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
	void JumpButtonPressed();
	void JumpingRelease();
	//For Equipping
	void Button01Pressed();
	void Button02Pressed();
	void Button03Pressed();
	void Button04Pressed();
	void Button05Pressed();
	//End For Equipping

	//Activate Abilities By Compare Tag
	void AbilityInputTagPressed(const FGameplayTag InputTag);
	void AbilityInputTagReleased(const FGameplayTag InputTag);
	void AbilityInputTagHeld(const FGameplayTag InputTag);

private:
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> MainInputContext;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> MoveInput;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> LookXAction;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> LookYAction;
	
	//For Generic Abilities
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> GenericInputContext;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> JumpAction;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> CrouchAction;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> InteractButton;
	//End Generic Abilities

	//For Equipping
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> Button01Action;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> Button02Action;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> Button03Action;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> Button04Action;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> Button05Action;
	//End For Equipping
	
	//For RuneAbilities
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> AbilitiesInputContext;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> StarterLMBAction;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> StarterRMBAction;
	//End RuneAbilities
	

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
	virtual void SetIsCastingDrawingWidget(bool bIsDrawing) override;

public:
	FORCEINLINE bool GetIsDrawingSpell() const {return bIsDrawingSpell;};

};


