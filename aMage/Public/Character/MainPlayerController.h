// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawMagic/Draw_PaintWidget.h"
#include "DrawMagic/UnistrokeRecognizer.h"
#include "Interact/TargetInterface.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UCombatActorComponent;
/**
 * 
 */
DECLARE_DELEGATE(FOnInteractButtonPressedSignature);
DECLARE_DELEGATE_OneParam(FOnFireButtonPressedSignature,bool);
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
	FOnFireButtonPressedSignature FireButtonPressedSignature;
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
	void FireButtonPressed(const FInputActionValue& Value);

	//TODO:: Change It For Widget
	void CursorTrace();


private:
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> MainInputContext;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveInput;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	UInputAction* LookXAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	UInputAction* LookYAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractButton;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	UInputAction* CastAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input , meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	
	ITargetInterface* LastActor;
	ITargetInterface* ThisActor;

	/*************************************************************************************************************/
	//DRAW FUNCTION IS HERE!

private:
	void StartDrawing(float DeltaTime);
	void LoadTemplates();
	void Spell();
private:
	//RightCLick
	bool bIsCastSpell;
	//LeftCLick
	bool bIsDrawing;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Draw , meta=(AllowPrivateAccess=true))
	TSoftObjectPtr<UDataTable> UnistrokeTable;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Draw , meta=(AllowPrivateAccess=true))
	TObjectPtr<UDraw_PaintWidget> PaintWidget;
	TSharedPtr<FUnistrokeRecognizer> Recognizer;
};


