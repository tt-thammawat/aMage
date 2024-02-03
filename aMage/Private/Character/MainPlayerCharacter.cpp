// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainPlayerCharacter.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Actor/PickUpEffectActor.h"
#include "Character/MainPlayerController.h"
#include "UI/HUD/MainPlayerHUD.h"
#include "Character/MainPlayerState.h"
#include "Character/PlayerAnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AMainPlayerCharacter::AMainPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	//Turn To That Movement
	GetCharacterMovement()->bOrientRotationToMovement =true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);
	
}

void AMainPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	AddCharacterAbilities();

}

void AMainPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

int32 AMainPlayerCharacter::GetPlayerLevel()
{
	const AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);
	return MainPlayerState->GetPlayerLevel();
}

void AMainPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMainPlayerCharacter,ItemData);
	DOREPLIFETIME(AMainPlayerCharacter,InteractObjectActor);
	DOREPLIFETIME(AMainPlayerCharacter,bFireButtonPressed);
}

void AMainPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

//Get ABS and AS From MainPlayerState
void AMainPlayerCharacter::InitAbilityActorInfo()
{
	AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);
	MainPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MainPlayerState,this);
	Cast<UBaseAbilitySystemComponent>(MainPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = MainPlayerState->GetAbilitySystemComponent();
	AttributeSet = MainPlayerState->GetPlayerStateAttributeSet();
	
	TrySetupHUD(MainPlayerState);
	InitDefaultAttributes();

}

void AMainPlayerCharacter::TrySetupHUD(AMainPlayerState* MainPlayerState)
{
	AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->InteractButtonPressedSignature.BindUObject(this,&ThisClass::InteractItemButtonPress);
		PlayerController->FireButtonPressedSignature.BindUObject(this,&ThisClass::FirePressed);
		AMainPlayerHUD* MainHUD = Cast<AMainPlayerHUD>(PlayerController->GetHUD());
		if (MainHUD)
		{
		 MainHUD->InitOverlay(PlayerController, MainPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

//Don't ForGet To Set ItemData ChangeHere
void AMainPlayerCharacter::OnRep_ItemDataChange()
{
	
}

//Interact Item Here
void AMainPlayerCharacter::InteractWithItem(AActor* InteractActor)
{
	if(HasAuthority())
	{
		InteractObjectActor= InteractActor;
	}
}

void AMainPlayerCharacter::InteractItemButtonPress()
{
	if(InteractObjectActor && InteractObjectActor->IsA<APickUpEffectActor>())
	{
		ServerEquipButtonPressed();
	}
}

void AMainPlayerCharacter::FirePressed(bool bPressed)
{
	if(!AbilitySystemComponent->HasMatchingGameplayTag
	(FGameplayTag::RequestGameplayTag(FName("Item.Equip.Staff")))) return;
	
	bFireButtonPressed = bPressed;
	if(bFireButtonPressed)
	{
		FHitResult HitResult;
		TraceUnderCrossHairs(HitResult);
		ServerFire(HitResult.ImpactPoint);
	}
}

void AMainPlayerCharacter::TraceUnderCrossHairs(FHitResult& TraceHitResult)
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
	UGameplayStatics::GetPlayerController(this,0),
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

void AMainPlayerCharacter::ServerFire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	MulticastFire(TraceHitTarget);
}

void AMainPlayerCharacter::MulticastFire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if(FireMontage)
	{
		PlayFireMontage();
	}
	FireSpell(TraceHitTarget);
}

void AMainPlayerCharacter::FireSpell(const FVector_NetQuantize& HitTarget)
{

}

void AMainPlayerCharacter::PlayFireMontage()
{
	UPlayerAnimInstance* AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if(AnimInstance && FireMontage)
	{
		AnimInstance->Montage_Play(FireMontage);
	}
}

void AMainPlayerCharacter::ServerEquipButtonPressed_Implementation()
{
	APickUpEffectActor* PickUpEffectActor = Cast<APickUpEffectActor>(InteractObjectActor);
	//Check GamePlayTag
	ItemData = PickUpEffectActor->GetItemData();
	const FGameplayTag EquipedStaffTag = FGameplayTag::RequestGameplayTag(FName("Item.Equip.Staff"));
	if(AbilitySystemComponent->HasMatchingGameplayTag(EquipedStaffTag)) return;
			if(ItemData.ItemTag.MatchesTag(EquipedStaffTag))
			{
				PickUpEffectActor->DestroyAfterPickUp(this);
				OnRep_ItemDataChange();
			}
}