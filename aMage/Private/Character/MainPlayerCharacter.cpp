// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainPlayerCharacter.h"
#include "GameplayTagsSingleton.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/MainGenericGameplayAbility.h"
#include "AbilitySystem/Data/RuneSpellClassInfo.h"
#include "Actor/InteractActor/MainEquipmentInteractActor.h"
#include "Character/MainPlayerController.h"
#include "UI/HUD/MainPlayerHUD.h"
#include "Character/MainPlayerState.h"
#include "Character/Inventory/Amage_EquipmentManager.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gamemode/MainGameMode.h"
#include "Interact/InteractInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
	DOREPLIFETIME_CONDITION(AMainPlayerCharacter,RefRuneTags,COND_OwnerOnly);
	DOREPLIFETIME(AMainPlayerCharacter,bIsAiming);
}

void AMainPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AimOffset(DeltaSeconds);
}

void AMainPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMainPlayerCharacter::AimOffset(float DeltaTime)
{
	FGameplayTagContainer GameplayTagContainer;
	if(GetAbilitySystemComponent())
	{
	GetAbilitySystemComponent()->GetOwnedGameplayTags(GameplayTagContainer);
	if(GameplayTagContainer.HasTag(FMainGameplayTags::Get().Item_Equip_Staff))
	{
		FVector Velocity = GetVelocity();
		Velocity.Z = 0.f;
		float Speed = Velocity.Size();
		bool bIsinAir = GetCharacterMovement()->IsFalling();

		if(Speed == 0.f && !bIsinAir)
		{
			FRotator CurrentAimRotation = FRotator(0.f,GetBaseAimRotation().Yaw,0.f);
			FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation,StartingAimRotation);
			AO_Yaw = DeltaAimRotation.Yaw;
		}
		
		if(Speed > 0.f || bIsinAir) // Running or Jumping Set Starting AimRotation To Set Yaw
		{
			StartingAimRotation = FRotator(0.f,GetBaseAimRotation().Yaw,0.f);
			AO_Yaw = 0.f;
		}

		AO_Pitch = GetBaseAimRotation().Pitch;
		if(AO_Pitch>90.f && !IsLocallyControlled())
		{
			//Map Pitch From [270-360) To Range [-90,0)
			FVector2D InRange(270.f,360.f);
			FVector2D OutRange(-90.f,0.f);
			// Clamp AO_Pitch To The OutPutRange
			AO_Pitch = FMath::GetMappedRangeValueClamped(InRange,OutRange,AO_Pitch);
		}
	}
	}
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

FVector AMainPlayerCharacter::GetCombatSocketLocation_Implementation()
{
	AActor* OutActor;
	PlayerEquipmentManager->GetItemInSlot("WeaponHandSocket",OutActor);
	const AMainEquipmentInteractActor* EquipmentInteractActor = Cast<AMainEquipmentInteractActor>(OutActor);
	FVector CombatSocketLocation = EquipmentInteractActor->GetSkeletalMeshComponent()->GetSocketLocation(EquipmentInteractActor->GetWeaponTipSocketName());
	return  CombatSocketLocation;
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

void AMainPlayerCharacter::MatchRuneSpellTags(TArray<FGameplayTag> RuneTags)
{
	RefRuneTags = RuneTags;
	
	if (HasAuthority())
	{
		ProcessAbilityRequest(RefRuneTags);
	}
	else
	{
		ServerRequestAbilityActivation(RefRuneTags);
	}
}

void AMainPlayerCharacter::ServerRequestAbilityActivation_Implementation(const TArray<FGameplayTag>& RuneTags)
{
	ProcessAbilityRequest(RuneTags);
}

void AMainPlayerCharacter::ProcessAbilityRequest(const TArray<FGameplayTag>& RuneTags)
{
	AMainGameMode* MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(MainGameMode)
	{
		TSubclassOf<UGameplayAbility> MatchedAbility = MainGameMode->RuneSpellClassInfos->GetRuneSpellMatchingAbility(RuneTags);
		UBaseAbilitySystemComponent* BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent());
		if(MatchedAbility)
		{
		TArray<TSubclassOf<UGameplayAbility>> AddAbilities;
		AddAbilities.Add(MatchedAbility);
		BaseAbilitySystemComponent->AddCharacterAbilities(AddAbilities);
		}
	}
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