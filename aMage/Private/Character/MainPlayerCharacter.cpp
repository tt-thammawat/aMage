// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainPlayerCharacter.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Actor/InteractActor/MainEquipmentInteractActor.h"
#include "Character/MainPlayerController.h"
#include "UI/HUD/MainPlayerHUD.h"
#include "Character/MainPlayerState.h"
#include "Character/Inventory/Amage_EquipmentManager.h"
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

	PlayerEquipmentManager = CreateDefaultSubobject<UAmage_EquipmentManager>("PlayerEquipmentManager");
	
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
		BindButtonToCharacter(PlayerController);
		if (AMainPlayerHUD* MainHUD = Cast<AMainPlayerHUD>(PlayerController->GetHUD()))
		{
			MainHUD->InitOverlay(PlayerController, MainPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

void AMainPlayerCharacter::BindButtonToCharacter(AMainPlayerController* PlayerController)
{
	PlayerController->InteractButtonPressedSignature.BindUObject(this,&ThisClass::InteractItemButtonPress);
	PlayerController->OnButtonPressed.BindUObject(this,&ThisClass::OnChangingButtonPressed);
}

FVector AMainPlayerCharacter::GetCombatSocketLocation()
{
	AActor* OutActor;
	PlayerEquipmentManager->GetItemInSlot(FName("WeaponHandSocket"),OutActor);
	const AMainEquipmentInteractActor* EquipmentInteractActor = Cast<AMainEquipmentInteractActor>(OutActor);
	const FVector WeaponSocketLocation = EquipmentInteractActor->GetSkeletalMeshComponent()->GetSocketLocation(EquipmentInteractActor->GetWeaponTipSocketName());
	return WeaponSocketLocation;
}

void AMainPlayerCharacter::AddItemAbilities(TSubclassOf<UGameplayAbility> AddItemAbility)
{
	UBaseAbilitySystemComponent* BaseAbilitySystemComponent = CastChecked<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	TArray<TSubclassOf<UGameplayAbility>> AddItemAbilities;
	AddItemAbilities.Add(AddItemAbility);
	
	if(!HasAuthority()) return;
	BaseAbilitySystemComponent->AddCharacterAbilities(AddItemAbilities);
}

void AMainPlayerCharacter::RemoveItemAbilities(TSubclassOf<UGameplayAbility> RemoveItemAbility)
{
	UBaseAbilitySystemComponent* BaseAbilitySystemComponent = CastChecked<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	TArray<TSubclassOf<UGameplayAbility>> RemoveItemAbilities;
	RemoveItemAbilities.Add(RemoveItemAbility);
	
	if(!HasAuthority()) return;
	BaseAbilitySystemComponent->RemoveCharacterAbilities(RemoveItemAbilities);
}


void AMainPlayerCharacter::SetInteractObjectActor(AActor* Actor)
{

	// Only set if there's no current interact object
	if (!InteractObjectActor)
	{
		InteractObjectActor = Actor;
		if(HasAuthority() && IsLocallyControlled())
		{
			if (IInteractInterface* InteractActor = Cast<IInteractInterface>(InteractObjectActor))
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
		
		if (IInteractInterface* InteractActor = Cast<IInteractInterface>(InteractObjectActor))
		{
			InteractActor->ShowInteractDetail();
		}
		
		if (IInteractInterface* OldInteractActor = Cast<IInteractInterface>(OldInteractObject))
		{
			OldInteractActor->HideInteractDetail();
		}
	}
	else if (InteractObjectActor == nullptr)
	{
		if (IInteractInterface* OldInteractActor = Cast<IInteractInterface>(OldInteractObject))
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
			if (IInteractInterface* InteractActor = Cast<IInteractInterface>(InteractObjectActor))
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

void AMainPlayerCharacter::ServerInteractButtonPressed_Implementation()
{
	if (IInteractInterface* InteractActor = Cast<IInteractInterface>(InteractObjectActor))
	{
		InteractActor->InteractWithItem(this);
	}
}