// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainPlayerCharacter.h"
#include "GameplayTagsSingleton.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/MainGenericGameplayAbility.h"
#include "AbilitySystem/Data/RuneSpellClassInfo.h"
#include "Actor/InteractActor/MainItemInteractActor.h"
#include "Character/MainPlayerController.h"
#include "UI/HUD/MainPlayerHUD.h"
#include "Character/MainPlayerState.h"
#include "Character/Inventory/Amage_EquipmentManager.h"
#include "Character/Inventory/MainAbilitiesItemComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gamemode/MainGameMode.h"
#include "Interact/InteractInterface.h"
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
	PlayerEquipmentManager->SetIsReplicated(true);
}

void AMainPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AMainPlayerCharacter,InteractObjectActor,COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AMainPlayerCharacter,AbilityHandle,COND_OwnerOnly);
	DOREPLIFETIME(AMainPlayerCharacter,bIsAiming);
}

void AMainPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	AddCharacterAbilities();
	if(HasAuthority())
	{
		StartEquipDefaultItem();
	}
}

void AMainPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
	ServerStartEquipDefaultItem();
}

int32 AMainPlayerCharacter::GetCharacterLevel()
{
	const AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);
	return MainPlayerState->GetCharacterLevel();
}

void AMainPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CalculateCrossHairSpreadByActor(DeltaSeconds);
	AimOffset(DeltaSeconds);
}

void AMainPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMainPlayerCharacter::ServerStartEquipDefaultItem_Implementation()
{
	StartEquipDefaultItem();
}

void AMainPlayerCharacter::AimOffset(float DeltaTime)
{
	if(GetAbilitySystemComponent())
	{
		FGameplayTagContainer GameplayTagContainer;
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

void AMainPlayerCharacter::CalculateCrossHairSpreadByActor(float DeltaTime)
{
	//Calculate CrossHair With Velocity
	FVector2D WalkSpreedRange{ 0.f,600.f };
	FVector2D VelocityMultiplierRange{ 0.f,1.f };
	FVector Velocity{ GetVelocity() };
	Velocity.Z = 0.f;
	
	//Calculate CrossHair In Air Factor
	if (GetCharacterMovement()->IsFalling())
	{
		//Spread The CrossHairs Slowly While In Air
		CrosshairInAirFactor = FMath::FInterpTo(
			CrosshairInAirFactor, 
			2.25f, 
			DeltaTime,
			2.25f);
	}
	else		//Charactor is On The Ground

	{
		//Shrink The Crosshairs Rapidly While On The Ground
		CrosshairInAirFactor = FMath::FInterpTo(
			CrosshairInAirFactor,
			0.0f,
			DeltaTime,
			30.f);

	}
	
	//True 0.05 Second After Firing
	if (bFiringSpell)
	{
		CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, .8f, DeltaTime, 15.0f);
	}
	else
	{
		CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.0f, DeltaTime, 15.0f);
	}

	CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpreedRange, VelocityMultiplierRange, Velocity.Size());
	CrosshairSpreadMultiplier = 
		0.5f + 
		CrosshairVelocityFactor + 
		CrosshairInAirFactor+
		CrosshairShootingFactor;
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
	PlayerController->OnToolbarButtonPressed.AddDynamic(this,&ThisClass::OnChangingButtonPressed);
	PlayerController->OnDropButtonPressed.AddDynamic(this,&ThisClass::OnDropButtonPressed);
}

FVector AMainPlayerCharacter::GetCombatSocketLocation_Implementation()
{
	FEquipment EquippedActor = PlayerEquipmentManager->EquipmentList[PlayerEquipmentManager->CurrentlyEquipIndex];
	if(EquippedActor.ItemActor)
	{
		const AMainItemInteractActor* EquipmentInteractActor = Cast<AMainItemInteractActor>(EquippedActor.ItemActor);
		FVector CombatSocketLocation = EquipmentInteractActor->GetSkeletalMeshComponent()->GetSocketLocation(EquipmentInteractActor->GetWeaponTipSocketName());
		return  CombatSocketLocation;
	}
	return FVector(0.f);
}

void AMainPlayerCharacter::AddItemAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AddItemAbilities,UObject* SourceObject)
{
	if(!HasAuthority()) return;
	
	bool bHaveAbilityWidget=false;
	
	AActor* SourceActor = Cast<AActor>(SourceObject);
	if (SourceActor)
	{
		UMainAbilitiesItemComponent* AbilityComponent = Cast<UMainAbilitiesItemComponent>(SourceActor->GetComponentByClass(UMainAbilitiesItemComponent::StaticClass()));

		if (AbilityComponent)
		{
			bHaveAbilityWidget = AbilityComponent->bHaveAbilityWidget;
		}
	}
	
	UBaseAbilitySystemComponent* BaseAbilitySystemComponent = CastChecked<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	BaseAbilitySystemComponent->AddCharacterAbilities(AddItemAbilities,SourceObject,bHaveAbilityWidget);
}

void AMainPlayerCharacter::RemoveItemAbilities(const TArray<TSubclassOf<UGameplayAbility>>& RemoveItemAbilities)
{
	UBaseAbilitySystemComponent* BaseAbilitySystemComponent = CastChecked<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	//Remove Item Abilities
	BaseAbilitySystemComponent->RemoveCharacterAbilities(RemoveItemAbilities);
	//Remove Normal Rune Spell Abilities
	BaseAbilitySystemComponent->RemoveNormalRuneSpellAbilities();
	//Remove Potion Abilities
	BaseAbilitySystemComponent->RemovePotionAbilities();
}

void AMainPlayerCharacter::MatchRuneSpellTags(TArray<FGameplayTag> RuneTags)
{
	
	if (HasAuthority())
	{
		ProcessAbilityRequest(RuneTags);
	}
	else
	{
		ServerRequestAbilityActivation(RuneTags);
	}
}

void AMainPlayerCharacter::ServerRequestAbilityActivation_Implementation(const TArray<FGameplayTag>& RuneTags)
{
	ProcessAbilityRequest(RuneTags);
}

void AMainPlayerCharacter::ProcessAbilityRequest(const TArray<FGameplayTag>& RuneTags)
{
	AMainGameMode* MainGameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());

	if(MainGameMode)
	{

		if (HasAuthority())
		{
			ProcessClearRuneSpellRequest();
		}
		else
		{
			ServerRequestClearRuneSpell();
		}
		
		UBaseAbilitySystemComponent* BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent());
		//Get Abilities From Server
		TSubclassOf<UGameplayAbility> MatchedAbility = MainGameMode->RuneSpellClassInfos->GetRuneSpellMatchingAbility(RuneTags);
		if(MatchedAbility)
		{
			TArray<TSubclassOf<UGameplayAbility>> AddAbilities;
			AddAbilities.Add(MatchedAbility);
			BaseAbilitySystemComponent->AddCharacterAbilities(AddAbilities,nullptr,true);
		}
	}
}

void AMainPlayerCharacter::ClearRuneSpell_Implementation()
{
	
		if (HasAuthority())
		{
			ProcessClearRuneSpellRequest();
		}
		else
		{
			ServerRequestClearRuneSpell();
		}
	
}

void AMainPlayerCharacter::ServerRequestClearRuneSpell_Implementation()
{
	ProcessClearRuneSpellRequest();
}

void AMainPlayerCharacter::ProcessClearRuneSpellRequest()
{
	UBaseAbilitySystemComponent* BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent());
		
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;

	for (const FGameplayAbilitySpec& Spec : BaseAbilitySystemComponent->GetActivatableAbilities())
	{
		//Remove Normal Spell
		if (Spec.Ability && Spec.Ability->AbilityTags.HasTagExact(FMainGameplayTags::Get().Ability_Rune_NormalSpell))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitiesToRemove)
	{
		BaseAbilitySystemComponent->ClearAbility(Handle);
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