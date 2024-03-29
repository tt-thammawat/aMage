// Fill out your copyright notice in the Description page of Project Settings.

 
#include "Character/BaseEnemy.h"

#include "GameplayTagsSingleton.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/MainAbilitySystemLibrary.h"
#include "AI/MainAIController.h"
#include "aMage/aMage.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Gamemode/MainGameMode.h"

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	Weapon->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");

	HealthBar=CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment((RootComponent));
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
}

void ABaseEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if(!HasAuthority()) return;
	//Set Ai Controller After Set Abilities
	MainAIController = Cast<AMainAIController>(NewController);

	MainAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	MainAIController->RunBehaviorTree(BehaviorTree);
	MainAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"),false);

	MainAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"),CharacterClass != ECharacterClass::Warrior);
}

void ABaseEnemy::HighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ABaseEnemy::UnHighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void ABaseEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ABaseEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

int32 ABaseEnemy::GetCharacterLevel()
{
	return Level;
}


void ABaseEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABaseEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	
	if(HasAuthority())
	{
		InitDefaultAttributes();
		UMainAbilitySystemLibrary::GiveStartUpAbilities(this,AbilitySystemComponent,CharacterClass);
	}
	BroadCastHealthValue();
	BindCallBackHealthValue();
}

void ABaseEnemy::InitDefaultAttributes() const
{
	UMainAbilitySystemLibrary::InitializeDefaultAttributes(this,CharacterClass,Level,AbilitySystemComponent);
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

}

void ABaseEnemy::BroadCastHealthValue()
{
	UBaseAttributeSet* BaseAttributes = Cast<UBaseAttributeSet>(GetAttributeSet());
	OnEnemyHealthChanged.Broadcast(BaseAttributes->GetHealth());
	OnEnemyMaxHealthChanged.Broadcast(BaseAttributes->GetMaxHealth());
}

void ABaseEnemy::BindCallBackHealthValue()
{
	UBaseAttributeSet* BaseAttributes = Cast<UBaseAttributeSet>(GetAttributeSet());

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributes->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnEnemyMaxHealthChanged.Broadcast(Data.NewValue);
	}
	);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributes->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnEnemyHealthChanged.Broadcast(Data.NewValue);
	}
	);
	
	//Receive HitReactTag HitReactTagChanged run
	AbilitySystemComponent->RegisterGameplayTagEvent(FMainGameplayTags::Get().Effects_HitReact,EGameplayTagEventType::NewOrRemoved).AddUObject(
	this,&ABaseEnemy::HitReactTagChanged);
}

void ABaseEnemy::HitReactTagChanged(const FGameplayTag CallBackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	
	if(NewCount >0)
	{
		bGotHit = true;
	}
	
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if(MainAIController && MainAIController->GetBlackboardComponent())
	{
		MainAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"),bHitReacting);
	}
}

void ABaseEnemy::Die(AActor* InstigatorActor)
{
	SetLifeSpan(LifeSpan);

	AMainGameMode* GM = GetWorld()->GetAuthGameMode<AMainGameMode>();
	if (GM)
	{
		GM->EnemyKilled(InstigatorActor);
	}
	if(MainAIController)
	{
		MainAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"),true);
	}
	
	Super::Die(InstigatorActor);
}