// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagsSingleton.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Data/RuneSpellClassInfo.h"
#include "GameFramework/Character.h"
#include "Gamemode/MainGameMode.h"
#include "Input/MainEnhancedInputComponent.h"
#include "Subsystem/KeyBindingManager.h"
#include "UI/Widget/DamageTextComponent.h"

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

void AMainPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter,
	bool bIsFireDamage, bool bIsLightningDamage, bool bIsIceDamage, bool bIsPhysicDamage)
{
	//IsValid Can Check both isNull And Pending Kill
	if(IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageTextComponent=  NewObject<UDamageTextComponent>(TargetCharacter,DamageTextComponentClass);
		//When Create Component Dynamically Must RegisterComponent
		DamageTextComponent->RegisterComponent();
		//Attach To Target That Taking Damage
		DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageTextComponent->SetDamageText(DamageAmount,bIsFireDamage,bIsLightningDamage,bIsIceDamage,bIsPhysicDamage);
	}
}

void AMainPlayerController::ServerRequestRuneAbilitiesLists_Implementation()
{
	AMainGameMode* GM = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		TArray<FRuneAbilityMapping> RuneAbilitiesLists = GM->RuneSpellClassInfos->AllRuneAbilities;
		ClientReceiveRuneAbilitiesLists(RuneAbilitiesLists);
	}
}

void AMainPlayerController::ClientReceiveRuneAbilitiesLists_Implementation(const TArray<FRuneAbilityMapping>& Abilities)
{
	OnRuneAbilitiesListReceived.Broadcast(Abilities);
}

void AMainPlayerController::ClientTriggerUIUpdate_Implementation(int CurrentlyButtonPressed)
{
	UpdatedWidgetUI.Broadcast(CurrentlyButtonPressed);
}

void AMainPlayerController::ServerRequestStaffAbilitiesLists_Implementation(const FString& AbilitiesName)
{
	AMainGameMode* GM = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		TArray<FRuneAbilityMapping> StaffAbilitiesLists = GM->RuneSpellClassInfos->StaffAbilities;
			TArray<FRuneAbilityMapping> AbilityMapping;
		for (FRuneAbilityMapping& Ability : StaffAbilitiesLists )
		{
			if(Ability.RuneSpellName == AbilitiesName)
			{
				AbilityMapping.Add(Ability);
			}
		}
		ClientReceiveStaffAbilitiesLists(AbilityMapping);
	}
}

void AMainPlayerController::ClientReceiveStaffAbilitiesLists_Implementation(const TArray<FRuneAbilityMapping>& Abilities)
{
	OnStaffAbilitiesListReceived.Broadcast(Abilities);
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
	MainEnhancedInputComponent->BindActionWithMappingSettings(MoveForward,ETriggerEvent::Triggered,this,&ThisClass::MoveForwardAction);
	MainEnhancedInputComponent->BindActionWithMappingSettings(MoveBackward,ETriggerEvent::Triggered,this,&ThisClass::MoveBackwardAction);
	MainEnhancedInputComponent->BindActionWithMappingSettings(MoveRight,ETriggerEvent::Triggered,this,&ThisClass::MoveRightAction);
	MainEnhancedInputComponent->BindActionWithMappingSettings(MoveLeft,ETriggerEvent::Triggered,this,&ThisClass::MoveLeftAction);
	
	MainEnhancedInputComponent->BindAction(LookXAction,ETriggerEvent::Triggered,this,&ThisClass::Turn);
	MainEnhancedInputComponent->BindAction(LookYAction,ETriggerEvent::Triggered,this,&ThisClass::LookUp);
	
	MainEnhancedInputComponent->BindActionWithMappingSettings(Button01Action,ETriggerEvent::Completed,this,&ThisClass::Button01Pressed);
	MainEnhancedInputComponent->BindActionWithMappingSettings(Button02Action,ETriggerEvent::Completed,this,&ThisClass::Button02Pressed);
	MainEnhancedInputComponent->BindActionWithMappingSettings(Button03Action,ETriggerEvent::Completed,this,&ThisClass::Button03Pressed);
	MainEnhancedInputComponent->BindActionWithMappingSettings(Button04Action,ETriggerEvent::Completed,this,&ThisClass::Button04Pressed);
	MainEnhancedInputComponent->BindActionWithMappingSettings(OptionButton,ETriggerEvent::Completed,this,&ThisClass::OptionButtonPressed);
	
	MainEnhancedInputComponent->BindActionWithMappingSettings(InteractButton,ETriggerEvent::Completed,this,&ThisClass::InteractButtonPressed);
	MainEnhancedInputComponent->BindActionWithMappingSettings(DropButton,ETriggerEvent::Completed,this,&ThisClass::DropButtonPressed);
	MainEnhancedInputComponent->BindActionWithMappingSettings(ScoreBoardButton,ETriggerEvent::Completed,this,&ThisClass::ScoreBoardButtonPressed);
	MainEnhancedInputComponent->BindActionWithMappingSettings(InventoryButton,ETriggerEvent::Completed,this,&ThisClass::InventoryButtonPressed);

	//Abilities
	MainEnhancedInputComponent->BindAbilityActions(InputConfig,this,&AMainPlayerController::AbilityInputTagPressed,&AMainPlayerController::AbilityInputTagReleased,&AMainPlayerController::AbilityInputTagHeld);
}

// Check with the FMainGameplayTags Singleton if the InputTag = InputTag.LMB,RMB
void AMainPlayerController::AbilityInputTagPressed(const FInputAbilityMapping InputAbilityMapping)
{
	
	FInputAbilityMapping Temp = InputAbilityMapping;
	if (UKeyBindingManager* KeyManager = GetGameInstance()->GetSubsystem<UKeyBindingManager>())
	{
		const FName InputPressName = Temp.MappingName ;
		KeyManager->OnKeyPressed(InputPressName);
	}
	
	if(!Temp.ActionInputTag.MatchesTagExact(FMainGameplayTags::Get().InputTag_LMB))
	{
		if(GetBaseAbilitySystemComponent())
		{
			GetBaseAbilitySystemComponent()->AbilityInputTagHeld(Temp.ActionInputTag);
		}
		return;
	}
	
	if(GetBaseAbilitySystemComponent())
	{
		GetBaseAbilitySystemComponent()->AbilityInputTagHeld(Temp.ActionInputTag);
	}
}

void AMainPlayerController::AbilityInputTagHeld(const FInputAbilityMapping InputAbilityMapping)
{
	FInputAbilityMapping Temp = InputAbilityMapping;
	if (UKeyBindingManager* KeyManager = GetGameInstance()->GetSubsystem<UKeyBindingManager>())
	{
		const FName InputPressName = Temp.MappingName ;
		KeyManager->OnKeyPressed(InputPressName);
	}
	
	if(!Temp.ActionInputTag.MatchesTagExact(FMainGameplayTags::Get().InputTag_LMB))
	{
		if(GetBaseAbilitySystemComponent())
		{
			GetBaseAbilitySystemComponent()->AbilityInputTagHeld(Temp.ActionInputTag);
		}
		return;
	}
	
	if(GetBaseAbilitySystemComponent())
	{
		GetBaseAbilitySystemComponent()->AbilityInputTagHeld(Temp.ActionInputTag);
	}
}

void AMainPlayerController::AbilityInputTagReleased(const FInputAbilityMapping InputAbilityMapping)
{
	FInputAbilityMapping Temp = InputAbilityMapping;
	if (UKeyBindingManager* KeyManager = GetGameInstance()->GetSubsystem<UKeyBindingManager>())
	{
		const FName InputPressName = Temp.MappingName ;
		KeyManager->OnKeyPressed(InputPressName);
	}
	
	if(!Temp.ActionInputTag.MatchesTagExact(FMainGameplayTags::Get().InputTag_LMB))
	{
		if(GetBaseAbilitySystemComponent())
		{
			GetBaseAbilitySystemComponent()->AbilityInputTagReleased(Temp.ActionInputTag);
		}
		return;
	}

	if(GetBaseAbilitySystemComponent())
	{
		GetBaseAbilitySystemComponent()->AbilityInputTagReleased(Temp.ActionInputTag);
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

void AMainPlayerController::MoveForwardAction(const FInputActionValue& InputActionValue,const FName InputMappingName)
{
	const float InputValue = InputActionValue.Get<float>();
	const FRotator YawRotation(0,GetControlRotation().Yaw,0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	//Send Button Pressed To KeyManager
	if (UKeyBindingManager* KeyManager = GetGameInstance()->GetSubsystem<UKeyBindingManager>())
	{
		const FName InputPressName = InputMappingName;
		KeyManager->OnKeyPressed(InputPressName);
	}

	if (!FMath::IsNearlyZero(InputValue))
	{
		if(APawn* ControlledPawn = GetPawn<APawn>())
		{
			ControlledPawn->AddMovementInput(ForwardDirection,InputValue);
		}
	}
}

void AMainPlayerController::MoveBackwardAction(const FInputActionValue& InputActionValue,const FName  InputMappingName)
{
	const float InputValue = InputActionValue.Get<float>();
	const FRotator YawRotation(0,GetControlRotation().Yaw,0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	if (UKeyBindingManager* KeyManager = GetGameInstance()->GetSubsystem<UKeyBindingManager>())
	{
		const FName InputPressName = InputMappingName;
		KeyManager->OnKeyPressed(InputPressName);
	}
	
	if (!FMath::IsNearlyZero(InputValue))
	{
		if(APawn* ControlledPawn = GetPawn<APawn>())
		{
			ControlledPawn->AddMovementInput(ForwardDirection,-InputValue);
		}
	}
}

void AMainPlayerController::MoveLeftAction(const FInputActionValue& InputActionValue,const FName  InputMappingName)
{
	const float InputValue = InputActionValue.Get<float>();
	const FRotator YawRotation(0,GetControlRotation().Yaw,0);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (UKeyBindingManager* KeyManager = GetGameInstance()->GetSubsystem<UKeyBindingManager>())
	{
		const FName InputPressName = InputMappingName;
		KeyManager->OnKeyPressed(InputPressName);
	}
	
	if (!FMath::IsNearlyZero(InputValue))
	{
		if(APawn* ControlledPawn = GetPawn<APawn>())
		{
			ControlledPawn->AddMovementInput(RightDirection,-InputValue);
		}
	}
}

void AMainPlayerController::MoveRightAction(const FInputActionValue& InputActionValue,const FName  InputMappingName)
{
	const float InputValue = InputActionValue.Get<float>();
	const FRotator YawRotation(0,GetControlRotation().Yaw,0);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (UKeyBindingManager* KeyManager = GetGameInstance()->GetSubsystem<UKeyBindingManager>())
	{
		const FName InputPressName = InputMappingName;
		KeyManager->OnKeyPressed(InputPressName);
	}
	
	if (!FMath::IsNearlyZero(InputValue))
	{
		if(APawn* ControlledPawn = GetPawn<APawn>())
		{
			ControlledPawn->AddMovementInput(RightDirection,InputValue);
		}
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

void AMainPlayerController::InteractButtonPressed(const FInputActionValue& InputActionValue,const FName  InputMappingName)
{

	if (UKeyBindingManager* KeyManager = GetGameInstance()->GetSubsystem<UKeyBindingManager>())
	{
		const FName InputPressName = InputMappingName;
		KeyManager->OnKeyPressed(InputPressName);
	}
	
	if(ACharacter* PlayerCharacter = this->GetCharacter())
	{
		if(PlayerCharacter)
		{
			InteractButtonPressedSignature.ExecuteIfBound();
		}
	}
}

void AMainPlayerController::DropButtonPressed(const FInputActionValue& InputActionValue,const FName  InputMappingName)
{
	if (UKeyBindingManager* KeyManager = GetGameInstance()->GetSubsystem<UKeyBindingManager>())
	{
		const FName InputPressName = InputMappingName;
		KeyManager->OnKeyPressed(InputPressName);
	}
	if(OnDropButtonPressed.IsBound())
	{
		OnDropButtonPressed.Broadcast();
	}
}

void AMainPlayerController::ScoreBoardButtonPressed(const FInputActionValue& InputActionValue,const FName  InputMappingName)
{
	if (UKeyBindingManager* KeyManager = GetGameInstance()->GetSubsystem<UKeyBindingManager>())
	{
		const FName InputPressName = InputMappingName;
		KeyManager->OnKeyPressed(InputPressName);
	}

	if(OnScoreBoardButtonPressed.IsBound())
	{
		OnScoreBoardButtonPressed.Broadcast();
	}
}

void AMainPlayerController::InventoryButtonPressed(const FInputActionValue& InputActionValue,const FName  InputMappingName)
{
	if (UKeyBindingManager* KeyManager = GetGameInstance()->GetSubsystem<UKeyBindingManager>())
	{
		const FName InputPressName = InputMappingName;
		KeyManager->OnKeyPressed(InputPressName);
	}
	
	if (OnInventoryButtonPressed.IsBound())
	{
		OnInventoryButtonPressed.Broadcast();
	}
}

void AMainPlayerController::OptionButtonPressed(const FInputActionValue& InputActionValue,const FName  InputMappingName)
{
	if (UKeyBindingManager* KeyManager = GetGameInstance()->GetSubsystem<UKeyBindingManager>())
	{
		const FName InputPressName = InputMappingName;
		KeyManager->OnKeyPressed(InputPressName);
	}
	
	if (OnOptionButtonPressed.IsBound())
	{
		OnOptionButtonPressed.Broadcast();
	}
}

void AMainPlayerController::Button01Pressed(const FInputActionValue& InputActionValue,const FName  InputMappingName)
{
	if (UKeyBindingManager* KeyManager = GetGameInstance()->GetSubsystem<UKeyBindingManager>())
	{
		const FName InputPressName = InputMappingName;
		KeyManager->OnKeyPressed(InputPressName);
	}

	if(OnToolbarButtonPressed.IsBound())
	{
		OnToolbarButtonPressed.Broadcast(0);
	}
}

void AMainPlayerController::Button02Pressed(const FInputActionValue& InputActionValue,const FName  InputMappingName)
{
	if (UKeyBindingManager* KeyManager = GetGameInstance()->GetSubsystem<UKeyBindingManager>())
	{
		const FName InputPressName = InputMappingName;
		KeyManager->OnKeyPressed(InputPressName);
	}

	if(OnToolbarButtonPressed.IsBound())
	{
		OnToolbarButtonPressed.Broadcast(1);
	}
}

void AMainPlayerController::Button03Pressed(const FInputActionValue& InputActionValue,const FName  InputMappingName)
{
	if (UKeyBindingManager* KeyManager = GetGameInstance()->GetSubsystem<UKeyBindingManager>())
	{
		const FName InputPressName = InputMappingName;
		KeyManager->OnKeyPressed(InputPressName);
	}

	if(OnToolbarButtonPressed.IsBound())
	{
		OnToolbarButtonPressed.Broadcast(2);
	}
}

void AMainPlayerController::Button04Pressed(const FInputActionValue& InputActionValue,const FName  InputMappingName)
{
	if (UKeyBindingManager* KeyManager = GetGameInstance()->GetSubsystem<UKeyBindingManager>())
	{
		const FName InputPressName = InputMappingName;
		KeyManager->OnKeyPressed(InputPressName);
	}

	if(OnToolbarButtonPressed.IsBound())
	{
		OnToolbarButtonPressed.Broadcast(3);
	}

}


