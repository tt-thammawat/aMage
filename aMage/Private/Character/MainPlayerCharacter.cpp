// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainPlayerCharacter.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Actor/PickUpEffectActor.h"
#include "Character/MainPlayerController.h"
#include "UI/HUD/MainPlayerHUD.h"
#include "Character/MainPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

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

int32 AMainPlayerCharacter::GetCharacterLevel()
{
	const AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);
	return MainPlayerState->GetCharacterLevel();
}

void AMainPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMainPlayerCharacter,ItemData);
	DOREPLIFETIME(AMainPlayerCharacter,InteractObjectActor);
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
		AMainPlayerHUD* MainHUD = Cast<AMainPlayerHUD>(PlayerController->GetHUD());
		if (MainHUD)
		{
			MainHUD->InitOverlay(PlayerController, MainPlayerState, AbilitySystemComponent, AttributeSet);
			MainHUD->InitDrawingWidget();
		}
	}
}

//Don't ForGet To Set ItemData ChangeHere
void AMainPlayerCharacter::OnRep_ItemDataChange()
{
	Weapon->SetSkeletalMesh(ItemData.ItemMesh);
}

//Interact Item Here
void AMainPlayerCharacter::InteractWithItem(AActor* InteractActor)
{
	if(HasAuthority())
	{
		InteractObjectActor= InteractActor;
	}
}

//TODO: Move This InteractShit to the GameplayAbilities Action 
void AMainPlayerCharacter::InteractItemButtonPress()
{
	if(InteractObjectActor && InteractObjectActor->IsA<APickUpEffectActor>())
	{
		ServerEquipButtonPressed();
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
				Weapon->SetSkeletalMesh(ItemData.ItemMesh);
				PickUpEffectActor->DestroyAfterPickUp(this);
				OnRep_ItemDataChange();
			}
}