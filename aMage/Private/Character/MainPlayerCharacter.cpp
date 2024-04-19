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
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
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

	OverheaderWidgetName=CreateDefaultSubobject<UWidgetComponent>("OverHeaderWidget");
	OverheaderWidgetName->SetupAttachment(RootComponent);
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
	StartEquipDefaultItem();
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

void AMainPlayerCharacter::Die(const AActor* InstigatorActor)
{
	if (HasAuthority())
	{
		FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
		EffectContextHandle.AddSourceObject(InstigatorActor);
		FGameplayAbilitySpec GameplayAbilitySpec(DeathGameplayClass, 1, INDEX_NONE, this);
		DeathSpecHandle = GetAbilitySystemComponent()->GiveAbilityAndActivateOnce(GameplayAbilitySpec);
	}
	OnDeath.Broadcast();
	
	MulticastHandleDeath();
}

void AMainPlayerCharacter::MulticastHandleDeath_Implementation()
{
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);

	GetMesh()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);

	if (OverheaderWidgetName)
	{
		OverheaderWidgetName->SetVisibility(false);
	}
	bDead = true;
}


void AMainPlayerCharacter::FinishedDead()
{
	GetMesh()->bPauseAnims = true;
}

void AMainPlayerCharacter::Revive_Implementation(const AActor* InstigatorActor)
{
	if (Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
	}

	MulticastRevive();
}


void AMainPlayerCharacter::MulticastRevive_Implementation()
{
	
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	GetMesh()->bPauseAnims = false;

	const float MontageDuration = PlayAnimMontage(ReviveAnimMontage, 1);
	
	if (MontageDuration > 0.0f)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// Bind the delegate to the OnMontageEnded event
			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindUObject(this, &AMainPlayerCharacter::OnReviveMontageEnded);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, ReviveAnimMontage);
		}
	}
}

void AMainPlayerCharacter::OnReviveMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (OverheaderWidgetName)
		{
			OverheaderWidgetName->SetVisibility(true);
		}

		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		if(HasAuthority())
		{
			if (GetAbilitySystemComponent() && DeathSpecHandle.IsValid())
			{
				GetAbilitySystemComponent()->ClearAbility(DeathSpecHandle);
			}
		}
		
		bDead = false;
	}
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
	else		//Character is On The Ground

	{
		//Shrink The Cross hairs Rapidly While On The Ground
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

FVector AMainPlayerCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	FName* SocketNamePtr = MappedSocketName.Find(MontageTag);
	
	FEquipment EquippedActor = PlayerEquipmentManager->EquipmentList[PlayerEquipmentManager->CurrentlyEquipIndex];
	if(EquippedActor.ItemActor)
	{
		const AMainItemInteractActor* EquipmentInteractActor = Cast<AMainItemInteractActor>(EquippedActor.ItemActor);
		FVector CombatSocketLocation = EquipmentInteractActor->GetSkeletalMeshComponent()->GetSocketLocation(EquipmentInteractActor->GetWeaponTipSocketName());
		return  CombatSocketLocation;
	}
	else
	{
		return GetMesh()->GetSocketLocation(*SocketNamePtr);
	}
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
		
		AActor* StaffActor;
		PlayerEquipmentManager->GetItemInSlot("0",StaffActor);
		
		TSubclassOf<UGameplayAbility> MatchedAbility;
		
		if(StaffActor)
		{
			UMainAbilitiesItemComponent* AbilitiesItemComponent = Cast<UMainAbilitiesItemComponent>(StaffActor->GetComponentByClass(UMainAbilitiesItemComponent::StaticClass()));
			if(AbilitiesItemComponent)
			{
				for(auto& Abilities : AbilitiesItemComponent->ItemAbilities)
				{
					if(RuneTags == Abilities.RuneSpellTags)
					{
						MatchedAbility = MainGameMode->RuneSpellClassInfos->GetStaffSpellMatchingAbility(RuneTags);
						if(MatchedAbility)
						{
							TArray<TSubclassOf<UGameplayAbility>> AddAbilities;
							AddAbilities.Add(MatchedAbility);
							BaseAbilitySystemComponent->AddCharacterAbilities(AddAbilities,nullptr,true);
							return;
						}
					}
				}
			}
		}
		
		//Get Abilities From Server
		MatchedAbility = MainGameMode->RuneSpellClassInfos->GetRuneSpellMatchingAbility(RuneTags);
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
	if (!BaseAbilitySystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent is not found."));
		return;
	}
	
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(FMainGameplayTags::Get().Ability_Rune_NormalSpell);

	TArray<FGameplayAbilitySpec*> MatchingAbilitySpecs;

	BaseAbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbilitySpecs, true);

	for (FGameplayAbilitySpec* Spec : MatchingAbilitySpecs)
	{
		if (Spec)
		{
			BaseAbilitySystemComponent->ClearAbility(Spec->Handle);
		}
	}
}

void AMainPlayerCharacter::ReloadRuneSpell_Implementation()
{
	if (HasAuthority())
	{
		ProcessReloadRuneSpellRequest();
	}
	else
	{
		ServerRequestReloadRuneSpell();
	}
}


void AMainPlayerCharacter::ServerRequestReloadRuneSpell_Implementation()
{
	ReloadRuneSpell_Implementation();
}

void AMainPlayerCharacter::ProcessReloadRuneSpellRequest()
{
	UBaseAbilitySystemComponent* BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent());
	if (!BaseAbilitySystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent is not found."));
		return;
	}

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(FMainGameplayTags::Get().Ability_Rune_NormalSpell);
	TArray<FGameplayAbilitySpec*> MatchingAbilitySpecs;

	BaseAbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbilitySpecs, true);
    
	for (FGameplayAbilitySpec* Spec : MatchingAbilitySpecs)
	{
		if (Spec && Spec->Ability)
		{
			TArray<UGameplayAbility*> AbilitiesInstance = Spec->GetAbilityInstances();
			
			for (UGameplayAbility* Ability : AbilitiesInstance)
			{
				UMainGenericGameplayAbility* MainGameplayAbility = Cast<UMainGenericGameplayAbility>(Ability);
				if(MainGameplayAbility)
				{
					MainGameplayAbility->RefillUsageTime();
				}
			}
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