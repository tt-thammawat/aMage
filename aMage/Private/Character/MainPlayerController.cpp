// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagsSingleton.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "DrawMagic/UnistrokeDataTable.h"
#include "Engine/UserInterfaceSettings.h"
#include "GameFramework/Character.h"
#include "Input/MainEnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"

AMainPlayerController::AMainPlayerController() :
bIsCastSpell(false),
bIsDrawing(false)
{
	//when took place on the server it send data to client
	bReplicates = true;

	//May be Move This And Change this SomeWhere If Performance Fuck
	//Construct Reconigzer Drawing System
	Recognizer = MakeShareable(new FUnistrokeRecognizer());

	static ConstructorHelpers::FObjectFinder<UDataTable> UnistrokeTemplatesTable(TEXT("DataTable'/Game/MagicDrawing/RegconizaDataTable.RegconizaDataTable'"));

	if (UnistrokeTemplatesTable.Succeeded())
	{
		UnistrokeTable = UnistrokeTemplatesTable.Object;
		LoadTemplates();
	}
}

void AMainPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController())
	{
		PaintWidget = CreateWidget<UDraw_PaintWidget>(this, UDraw_PaintWidget::StaticClass());
		if (PaintWidget != nullptr)
		{
			PaintWidget->AddToViewport();
			PaintWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	
	check(MainInputContext);
	
	//Set New Enhanced Input For Local Player
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()); 

	if(Subsystem)
	{
		Subsystem->AddMappingContext(MainInputContext,0);
	}

	
}

void AMainPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	if(bIsCastSpell)
	{
		if(IsLocalController())
		{
			StartDrawing(DeltaTime);
		}
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UMainEnhancedInputComponent* MainEnhancedInputComponent = CastChecked<UMainEnhancedInputComponent>(InputComponent);
	
	MainEnhancedInputComponent->BindAction(MoveInput,ETriggerEvent::Triggered,this,&ThisClass::MoveAction);
	MainEnhancedInputComponent->BindAction(LookXAction,ETriggerEvent::Triggered,this,&ThisClass::Turn);
	MainEnhancedInputComponent->BindAction(LookYAction,ETriggerEvent::Triggered,this,&ThisClass::LookUp);
	MainEnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Started,this,&ThisClass::JumpButtonPressed);
	MainEnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Canceled,this,&ThisClass::JumpingRelease);
	MainEnhancedInputComponent->BindAction(CrouchAction,ETriggerEvent::Completed,this,&ThisClass::CrouchButtonPressed);
	MainEnhancedInputComponent->BindAction(InteractButton,ETriggerEvent::Completed,this,&ThisClass::InteractButtonPressed);

	MainEnhancedInputComponent->BindAbilityActions(InputConfig,this,&AMainPlayerController::AbilityInputTagPressed,&AMainPlayerController::AbilityInputTagReleased,&AMainPlayerController::AbilityInputTagHeld);
	
}

// Check with the FMainGameplayTags Singleton if the InputTag = InputTag.LMB,RMB
void AMainPlayerController::AbilityInputTagPressed(const FGameplayTag InputTag)
{

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
	if(bIsCastSpell == false)
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
	if(bIsCastSpell == false)
	{
		const FVector2D LookAxisVector = Value.Get<FVector2D>();

		if(APawn* ControlledPawn = GetPawn<APawn>())
		{
			ControlledPawn->AddControllerPitchInput(LookAxisVector.X);
		}
	}
}

//TODO:InteractButton
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

void AMainPlayerController::CrouchButtonPressed()
{

	if(ACharacter* PlayerCharacter = this->GetCharacter())
	{
		if(PlayerCharacter->bIsCrouched)
		{
			PlayerCharacter->UnCrouch();
		}
		else
		{
			PlayerCharacter->Crouch();
		}
	}
}

void AMainPlayerController::CastButtonPressed(const FInputActionValue& Value)
{
		const bool IsAiming = Value.Get<bool>();
		if(IsAiming)
		{
			bIsCastSpell = true;
			if(IsLocalController())
			{
				FInputModeGameAndUI InputMode;
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				InputMode.SetHideCursorDuringCapture(false);
				PaintWidget->SetVisibility(ESlateVisibility::Visible);
				bShowMouseCursor = true;
			}
		}
		else
		{
			bIsCastSpell = false;
			if(IsLocalController())
			{
				FInputModeGameOnly InputModeGame;
				SetInputMode(InputModeGame);
			
				bShowMouseCursor = false;
				DefaultMouseCursor = EMouseCursor::Crosshairs;
				PaintWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
				PaintWidget->RemoveAllPoints();
			}
			bIsDrawing = false;

		}
		//TODO:: Implement Aiming From Combat Component
}


void AMainPlayerController::JumpButtonPressed()
{
	if(ACharacter* PlayerCharacter = this->GetCharacter())
	{
		if(PlayerCharacter->bIsCrouched)
		{
			PlayerCharacter->UnCrouch();
		}
		else
		{
			PlayerCharacter->Jump();
		}
	}
}

void AMainPlayerController::JumpingRelease()
{
	if(ACharacter* PlayerCharacter = this->GetCharacter())
	{
		PlayerCharacter->StopJumping();
	}
}

//TODO:: Implement FireButton
void AMainPlayerController::FireButtonPressed(const FInputActionValue& Value)
{
	const bool IsClick = Value.Get<bool>();

	if(bIsCastSpell)
	{
		if(IsClick)
		{
			bIsDrawing = true;
		}
		else
		{

			bIsDrawing = false;
			if(IsLocalController())
			{
				Spell();
			}
		}
		
	}
}

void AMainPlayerController::TraceUnderCrossHair(FHitResult& TraceHitResult)
{
	FVector2D ViewPortSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}
	//Center Of The Viewport Vector
	const FVector2D CrossHairLocation(ViewPortSize.X/2,ViewPortSize.Y/2);
	FVector CrossHairWorldPosition;
	FVector CrossHairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
	//Get PlayerController
	this,
	CrossHairLocation,
	CrossHairWorldPosition,
	CrossHairWorldDirection
	);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, MyPawn);
	if(bScreenToWorld)
	{
		//LineTrace
		//Center Of The Screen
		const FVector Start = CrossHairWorldPosition;
		//Start + Direction + 80000
		const FVector End = Start + CrossHairWorldDirection * 80000.f;
		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECC_Visibility,
			TraceParams
			);

		if(!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
		}
		
	}
}

void AMainPlayerController::StartDrawing(float DeltaTime)
{
	if(bIsDrawing)
	{
		const TArray<FVector2D> Points = PaintWidget->GetPoints();
		float MouseX = 0.0f;
		float MouseY = 0.0f;

		GetMousePosition(MouseX, MouseY);

		const FVector2D MousePoint = FVector2D(MouseX, MouseY);
		const FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		const float viewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportSize.X, viewportSize.Y));		
		const FVector2D ScaledPoint = MousePoint / viewportScale;
		const FVector2D LastPoint = Points.Num() > 0 ? Points.Last() : ScaledPoint;
		const bool IsNewPoint = !LastPoint.Equals(ScaledPoint, 1.0f);

		if (Points.Num() == 0 || (Points.Num() > 0 && IsNewPoint))
		{
			PaintWidget->AddPoint(ScaledPoint);
		}
	}
}

//Do The Async Load Here Prepare To Die
void AMainPlayerController::LoadTemplates()
{
	if (UnistrokeTable != nullptr)
	{
		const FString ContextString = "Templates";
		TArray<FUnistrokeDataTable*> Rows;

		UnistrokeTable->GetAllRows<FUnistrokeDataTable>(ContextString, Rows);

		for (int i = 0; i < Rows.Num(); i++)
		{
			Recognizer->AddTemplate((*Rows[i]).Name, (*Rows[i]).Points);
		}
	}
}

//Recognize Drawing Here
void AMainPlayerController::Spell()
{
	TArray<FVector2D>CurrentPoints = PaintWidget->GetPoints();
	FUnistrokeResult Result = Recognizer->Recognize(CurrentPoints, false);
	
	if (Result.Score < 0.8f)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Magic", true, FVector2D(2, 2));
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Result.Name, true, FVector2D(2, 2));
		//TODO::Add Success Casting Condition Here  Here
	}

	PaintWidget->RemoveAllPoints();

}

