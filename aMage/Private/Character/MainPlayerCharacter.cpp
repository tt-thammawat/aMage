// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainPlayerCharacter.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Actor/InteractActor/MainEquipmentInteractActor.h"
#include "Character/MainPlayerController.h"
#include "UI/HUD/MainPlayerHUD.h"
#include "Character/MainPlayerState.h"
#include "Components/AGR_InventoryManager.h"
#include "Components/AGR_ItemComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interact/InteractInterface.h"

AMainPlayerCharacter::AMainPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	//Turn To That Movement
	GetCharacterMovement()->bOrientRotationToMovement =true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);

	EquipmentManager = CreateDefaultSubobject<UAGR_EquipmentManager>("EquipmentManager");
	InventoryManager = CreateDefaultSubobject<UAGR_InventoryManager>("InventoryManager");

	
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
	DOREPLIFETIME_CONDITION(AMainPlayerCharacter,InteractObjectActor,COND_OwnerOnly);
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
	if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController()))
	{
		PlayerController->InteractButtonPressedSignature.BindUObject(this,&ThisClass::InteractItemButtonPress);
		if (AMainPlayerHUD* MainHUD = Cast<AMainPlayerHUD>(PlayerController->GetHUD()))
		{
			MainHUD->InitOverlay(PlayerController, MainPlayerState, AbilitySystemComponent, AttributeSet);
			MainHUD->InitDrawingWidget(PlayerController);
		}
	}
}

FVector AMainPlayerCharacter::GetCombatSocketLocation()
{
	AActor* OutActor;
	EquipmentManager->GetItemInSlot(FName("WeaponHandSocket"),OutActor);
	AMainEquipmentInteractActor* EquipmentInteractActor = Cast<AMainEquipmentInteractActor>(OutActor);
	FVector WeaponSocketLocation = EquipmentInteractActor->GetSkeletalMeshComponent()->GetSocketLocation(EquipmentInteractActor->GetWeaponTipSocketName());
	return WeaponSocketLocation;
}

void AMainPlayerCharacter::AddItemAbilities() const
{
	UBaseAbilitySystemComponent* BaseAbilitySystemComponent = CastChecked<UBaseAbilitySystemComponent>(AbilitySystemComponent);

	if(!HasAuthority()) return;
//TODO : Change This To Equip Item Abilities
//	BaseAbilitySystemComponent->AddCharacterAbilities(StartUpAbilities);
}

void AMainPlayerCharacter::RemoveItemAbilities() const
{
	UBaseAbilitySystemComponent* BaseAbilitySystemComponent = CastChecked<UBaseAbilitySystemComponent>(AbilitySystemComponent);

	if(!HasAuthority()) return;
	//TODO : Change This To Equip Item Abilities
	BaseAbilitySystemComponent->RemoveCharacterAbilities(StartUpAbilities);
}


void AMainPlayerCharacter::SetInteractObjectActor(AActor* Actor)
{

	// Only set if there's no current interact object
	if (!InteractObjectActor)
	{
		InteractObjectActor = Actor;
		if(HasAuthority() && IsLocallyControlled())
		{
			IInteractInterface* InteractActor = Cast<IInteractInterface>(InteractObjectActor);
			if (InteractActor)
			{
				InteractActor->ShowInteractDetail();
			}
		}
	}
}

void AMainPlayerCharacter::OnRep_InteractObjectActor(AActor* OldInteractObject)
{
	if(InteractObjectActor != OldInteractObject)
	{
		IInteractInterface* InteractActor = Cast<IInteractInterface>(InteractObjectActor);
		if (InteractActor)
		{
			InteractActor->ShowInteractDetail();
		}
		
		IInteractInterface* OldInteractActor = Cast<IInteractInterface>(OldInteractObject);
		if (OldInteractActor)
		{
			OldInteractActor->HideInteractDetail();
		}
	}
	else if (InteractObjectActor == nullptr)
	{
		IInteractInterface* OldInteractActor = Cast<IInteractInterface>(OldInteractObject);
		if (OldInteractActor)
		{
			OldInteractActor->HideInteractDetail();
		}
	}
}

void AMainPlayerCharacter::ClearInteractObjectActor(AActor* Actor)
{
	// Only clear if the leaving actor is the current interact object
	if (InteractObjectActor == Actor)
	{
		if(HasAuthority() && IsLocallyControlled())
		{
			IInteractInterface* InteractActor = Cast<IInteractInterface>(InteractObjectActor);
			if (InteractActor)
			{
				InteractActor->HideInteractDetail();
			}
		}
		InteractObjectActor = nullptr;
	}
}

void AMainPlayerCharacter::InteractItemButtonPress()
{
	if(InteractObjectActor)
	{
		ServerInteractButtonPressed();
	}
}



//TODO: Fix Item Data
void AMainPlayerCharacter::ServerInteractButtonPressed_Implementation()
{
	IInteractInterface* InteractActor = Cast<IInteractInterface>(InteractObjectActor);
	if (InteractActor)
	{
		InteractActor->InteractWithItem(this);
	}

}

// //Equip This if That Item have AGR_ItemComponent
// if(UAGR_ItemComponent* ItemComponent = InteractObjectActor->FindComponentByClass<UAGR_ItemComponent>())
// {
// 	ItemComponent->PickUpItem(InventoryManager);
// }
//
// // Cast some kind of buff when pick up it attach to ASC
// else if(APickUpEffectActor* PickUpEffectActor = Cast<APickUpEffectActor>(InteractObjectActor))
// {
// //Check GamePlayTag
// ItemData = PickUpEffectActor->GetItemData();
// const FGameplayTag EquippedStaffTag = FGameplayTag::RequestGameplayTag(FName("Item.Equip.Staff"));
// if(AbilitySystemComponent->HasMatchingGameplayTag(EquippedStaffTag)) return;
// 		if(ItemData.ItemTag.MatchesTag(EquippedStaffTag))
// 		{
// 		//	Weapon->SetSkeletalMesh(ItemData.ItemMesh);
// 			PickUpEffectActor->DestroyAfterPickUp(this);
// 			OnRep_ItemDataChange();
// 		}
// 	
// }