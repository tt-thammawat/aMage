// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/RuneSpellClassInfo.h"
#include "Interact/TargetInterface.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

struct FInputAbilityMapping;
class UPlayerMappableKeySettings;
class UMainInputConfig;
class UDamageTextComponent;
class UInputAction;
class UBaseAbilitySystemComponent;
struct FGameplayTag;
/**
 * 
 */
DECLARE_DELEGATE(FOnInteractButtonPressedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOptionButtonPressedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDropButtonPressedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScoreBoardButtonPressedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryButtonPressedSignature);

//Tool Bar Slot Pressed 1 2 3 4
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonPressedSignature,int,ButtonPressedNumber);
//Request Rune Ability List For UI
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRuneAbilitiesListReceived, const TArray<FRuneAbilityMapping>&, Abilities);
//Request Staff ability List
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaffAbilitiesListReceived, const TArray<FRuneAbilityMapping>&, Abilities);
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
	FOnStaffAbilitiesListReceived OnStaffAbilitiesListReceived;
	UPROPERTY(BlueprintAssignable)
	FOnOptionButtonPressedSignature OnOptionButtonPressed;
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

	//Get StaffAbilities If Exists
	UFUNCTION(Server, Reliable,BlueprintCallable)
	void ServerRequestStaffAbilitiesLists(const FString& AbilitiesName);

	UFUNCTION(Client, Reliable)
	void ClientReceiveStaffAbilitiesLists(const TArray<FRuneAbilityMapping>& Abilities);
	
	UPROPERTY(BlueprintAssignable)
	FOnRuneAbilitiesListReceived OnRuneAbilitiesListReceived;
protected:
	
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void PostInitializeComponents() override;
	void MoveForwardAction(const FInputActionValue& InputActionValue,const FName  InputMappingName);
	void MoveBackwardAction(const FInputActionValue& InputActionValue,const FName InputMappingName);
	void MoveLeftAction(const FInputActionValue& InputActionValue,const FName InputMappingName);
	void MoveRightAction(const FInputActionValue& InputActionValue,const FName InputMappingName);

	void Turn(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void InteractButtonPressed(const FInputActionValue& InputActionValue,const FName  InputMappingName);
	void DropButtonPressed(const FInputActionValue& InputActionValue,const FName  InputMappingName);
	UFUNCTION(BlueprintCallable)
	void ScoreBoardButtonPressed(const FInputActionValue& InputActionValue,const FName  InputMappingName);
	void InventoryButtonPressed(const FInputActionValue& InputActionValue,const FName  InputMappingName);
	void OptionButtonPressed(const FInputActionValue& InputActionValue,const FName  InputMappingName);
	
	//For Equipping
	void Button01Pressed(const FInputActionValue& InputActionValue,const FName  InputMappingName);
	void Button02Pressed(const FInputActionValue& InputActionValue,const FName  InputMappingName);
	void Button03Pressed(const FInputActionValue& InputActionValue,const FName  InputMappingName);
	void Button04Pressed(const FInputActionValue& InputActionValue,const FName  InputMappingName);
	//End For Equipping

	//Activate Abilities By Compare Tag
	void AbilityInputTagPressed(const FInputAbilityMapping InputAbilityMapping);
	void AbilityInputTagReleased(const FInputAbilityMapping InputAbilityMapping);
	void AbilityInputTagHeld(const FInputAbilityMapping InputAbilityMapping);

	UPROPERTY(EditDefaultsOnly,Category = "Input")
	TObjectPtr<UMainInputConfig> InputConfig;
	
private:
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> MainInputContext;
	//Movement
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveForward;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveBackward;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveLeft;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveRight;
	//Look
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookXAction;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookYAction;
	
	//For Generic Abilities
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> GenericInputContext;
	//End Generic Abilities
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractButton;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DropButton;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ScoreBoardButton;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InventoryButton;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> OptionButton;
	//For Equipping
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Button01Action;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Button02Action;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Button03Action;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Button04Action;
	//End For Equipping
	
	//For RuneAbilities
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> AbilitiesInputContext;
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


