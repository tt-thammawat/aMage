// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/RuneSpellClassInfo.h"
#include "Interact/TargetInterface.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UDamageTextComponent;
class UMainInputAction;
class UBaseAbilitySystemComponent;
struct FGameplayTag;
/**
 * 
 */
DECLARE_DELEGATE(FOnInteractButtonPressedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDropButtonPressedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScoreBoardButtonPressedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryButtonPressedSignature);

//Tool Bar Slot Pressed 1 2 3 4
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonPressedSignature,int,ButtonPressedNumber);
//Request Rune Ability List For UI
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRuneAbilitiesListReceived, const TArray<FRuneAbilityMapping>&, Abilities);
//UpDate UI When PickUp Something
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdatedWidgetUISignature,int,CurrentlyPressedButton);

class ITargetInterface;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

UCLASS()
class AMAGE_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMainPlayerController();
	
	FOnInteractButtonPressedSignature InteractButtonPressedSignature;
	UPROPERTY(BlueprintAssignable)
	FOnButtonPressedSignature OnToolbarButtonPressed;
	UPROPERTY(BlueprintAssignable)
	FOnDropButtonPressedSignature OnDropButtonPressed;
	UPROPERTY(BlueprintAssignable)
	FOnScoreBoardButtonPressedSignature OnScoreBoardButtonPressed;
	UPROPERTY(BlueprintAssignable)
	FOnInventoryButtonPressedSignature OnInventoryButtonPressed;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdatedWidgetUISignature UpdatedWidgetUI;
	UFUNCTION(Client,Reliable,BlueprintCallable, Category="YourCategory")
	void ClientTriggerUIUpdate(int CurrentlyButtonPressed);
	
	UFUNCTION(Client,Reliable)
	void ShowDamageNumber(float DamageAmount,ACharacter* TargetCharacter,bool bIsFireDamage,bool bIsLightningDamage,bool bIsIceDamage,bool bIsPhysicDamage);
	
	//Get Abilities For Drawing Widget
    UFUNCTION(Server, Reliable,BlueprintCallable)
	void ServerRequestRuneAbilitiesLists();
	
	UFUNCTION(Client, Reliable)
	void ClientReceiveRuneAbilitiesLists(const TArray<FRuneAbilityMapping>& Abilities);

	UPROPERTY(BlueprintAssignable)
	FOnRuneAbilitiesListReceived OnRuneAbilitiesListReceived;
protected:
	
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void PostInitializeComponents() override;
	void MoveAction(const FInputActionValue& InputActionValue) ;
	void Turn(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void InteractButtonPressed();
	void DropButtonPressed();
	void ScoreBoardButtonPressed();
	void InventoryButtonPressed();

	//For Equipping
	void Button01Pressed();
	void Button02Pressed();
	void Button03Pressed();
	void Button04Pressed();
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
	//End Generic Abilities
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> InteractButton;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> DropButton;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> ScoreBoardButton;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> InventoryButton;
	//For Equipping
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> Button01Action;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> Button02Action;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> Button03Action;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainInputAction> Button04Action;
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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
	
	ITargetInterface* LastActor;
	ITargetInterface* ThisActor;

public:
	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent();

	/*************************************************************************************************************/
	//DRAW FUNCTION IS HERE!

private:
	bool bIsDrawingSpell;

public:
	void SetbIsDrawingSpell(bool bIsDrawing) {bIsDrawingSpell = bIsDrawing;};
	FORCEINLINE bool GetIsDrawingSpell() const {return bIsDrawingSpell;};

};


