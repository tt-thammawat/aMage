// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagsSingleton.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Input/MainEnhancedInputComponent.h"

AMainPlayerController::AMainPlayerController() :
bIsDrawingSpell(false)
{
	//when took place on the server it send data to client
	bReplicates = true;
	//May be Move This And Change this SomeWhere If Performance Fuck

}

void AMainPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	
	check(MainInputContext);
	check(AbilitiesInputContext);
	check(GenericInputContext);
	//Set New Enhanced Input For Local Player
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()); 

	if(Subsystem)
	{
		Subsystem->AddMappingContext(MainInputContext,0);
		Subsystem->AddMappingContext(GenericInputContext,0);
		Subsystem->AddMappingContext(AbilitiesInputContext,0);

	}

}

void AMainPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UMainEnhancedInputComponent* MainEnhancedInputComponent = CastChecked<UMainEnhancedInputComponent>(InputComponent);
	MainEnhancedInputComponent->BindAction(MoveInput,ETriggerEvent::Triggered,this,&ThisClass::MoveAction);
	MainEnhancedInputComponent->BindAction(LookXAction,ETriggerEvent::Triggered,this,&ThisClass::Turn);
	MainEnhancedInputComponent->BindAction(LookYAction,ETriggerEvent::Triggered,this,&ThisClass::LookUp);
	MainEnhancedInputComponent->BindAction(Button01Action,ETriggerEvent::Completed,this,&ThisClass::Button01Pressed);
	MainEnhancedInputComponent->BindAction(Button02Action,ETriggerEvent::Completed,this,&ThisClass::Button02Pressed);
	MainEnhancedInputComponent->BindAction(Button03Action,ETriggerEvent::Completed,this,&ThisClass::Button03Pressed);
	MainEnhancedInputComponent->BindAction(Button04Action,ETriggerEvent::Completed,this,&ThisClass::Button04Pressed);
	MainEnhancedInputComponent->BindAction(Button05Action,ETriggerEvent::Completed,this,&ThisClass::Button05Pressed);
	
	//Abilities
	MainEnhancedInputComponent->BindAbilityActions(GenericInputContext,this,&AMainPlayerController::AbilityInputTagPressed,&AMainPlayerController::AbilityInputTagReleased,&AMainPlayerController::AbilityInputTagHeld);
	MainEnhancedInputComponent->BindAction(InteractButton,ETriggerEvent::Completed,this,&ThisClass::InteractButtonPressed);
	MainEnhancedInputComponent->BindAbilityActions(AbilitiesInputContext,this,&AMainPlayerController::AbilityInputTagPressed,&AMainPlayerController::AbilityInputTagReleased,&AMainPlayerController::AbilityInputTagHeld);
}

// Check with the FMainGameplayTags Singleton if the InputTag = InputTag.LMB,RMB
void AMainPlayerController::AbilityInputTagPressed(const FGameplayTag InputTag)
{
	
	if(!InputTag.MatchesTagExact(FMainGameplayTags::Get().InputTag_LMB))
	{
		if(GetBaseAbilitySystemComponent())
		{
			GetBaseAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	
	if(GetBaseAbilitySystemComponent())
	{
		GetBaseAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
	}
}

void AMainPlayerController::AbilityInputTagHeld(const FGameplayTag InputTag)
{
	if(!InputTag.MatchesTagExact(FMainGameplayTags::Get().InputTag_LMB))
	{
		if(GetBaseAbilitySystemComponent())
		{
			GetBaseAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	
	if(GetBaseAbilitySystemComponent())
	{
		GetBaseAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
	}
}

void AMainPlayerController::AbilityInputTagReleased(const FGameplayTag InputTag)
{
	
	if(!InputTag.MatchesTagExact(FMainGameplayTags::Get().InputTag_LMB))
	{
		if(GetBaseAbilitySystemComponent())
		{
			GetBaseAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
		}
		return;
	}

	if(GetBaseAbilitySystemComponent())
	{
		GetBaseAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
	}
}

UBaseAbilitySystemComponent* AMainPlayerController::GetBaseAbilitySystemComponent()
{
	if(BaseAbilitySystemComponent == nullptr)
	{
		BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return BaseAbilitySystemComponent;
}


void AMainPlayerController::MoveAction(const FInputActionValue& InputActionValue)
{
	const FVector InputAxisVector = InputActionValue.Get<FVector>();

	const FRotator YawRotation(0,GetControlRotation().Yaw,0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
}

void AMainPlayerController::Turn(const FInputActionValue& Value)
{
	if(bIsDrawingSpell == false)
	{
		const FVector2D LookAxisVector = Value.Get<FVector2D>();

		if(APawn* ControlledPawn = GetPawn<APawn>())
		{
			ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		}
	}
}

void AMainPlayerController::LookUp(const FInputActionValue& Value)
{
	if(bIsDrawingSpell == false)
	{
		const FVector2D LookAxisVector = Value.Get<FVector2D>();

		if(APawn* ControlledPawn = GetPawn<APawn>())
		{
			ControlledPawn->AddControllerPitchInput(LookAxisVector.X);
		}
	}
}

void AMainPlayerController::InteractButtonPressed()
{
	if(ACharacter* PlayerCharacter = this->GetCharacter())
	{
		if(PlayerCharacter)
		{
			InteractButtonPressedSignature.ExecuteIfBound();
		}
	}
}

void AMainPlayerController::Button01Pressed()
{
	OnButtonPressed.ExecuteIfBound(1);
}

void AMainPlayerController::Button02Pressed()
{
	OnButtonPressed.ExecuteIfBound(2);
}

void AMainPlayerController::Button03Pressed()
{
	OnButtonPressed.ExecuteIfBound(3);
}

void AMainPlayerController::Button04Pressed()
{
	OnButtonPressed.ExecuteIfBound(4);

}

void AMainPlayerController::Button05Pressed()
{
	OnButtonPressed.ExecuteIfBound(5);
}



