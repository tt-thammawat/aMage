// Fill out your copyright notice in the Description page of Project Settings.

 
#include "Character/BaseEnemy.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/MainAbilitySystemLibrary.h"
#include "aMage/aMage.h"
#include "Components/WidgetComponent.h"

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	Weapon->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");

	HealthBar=CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment((RootComponent));
}

void ABaseEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}


void ABaseEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);

}

int32 ABaseEnemy::GetCharacterLevel()
{
	return Level;
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
}

void ABaseEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABaseEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitDefaultAttributes();
	BroadCastHealthValue();
	BindCallBackHealthValue();
}

void ABaseEnemy::InitDefaultAttributes() const
{
	UMainAbilitySystemLibrary::InitializeDefaultAttributes(this,CharacterClass,Level,AbilitySystemComponent);
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
}
