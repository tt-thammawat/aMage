// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"


void UOverlayWidgetController::BroadcastInitialValue()
{
	const UBaseAttributeSet* MageAttributeSet = CastChecked<UBaseAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(MageAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(MageAttributeSet->GetMaxHealth());
	OnManaChange.Broadcast(MageAttributeSet->GetMana());
	OnMaxmanaChange.Broadcast(MageAttributeSet->GetMaxMana());
}

// When Value Change I call GAS Delegate
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UBaseAttributeSet* MageAttributeSet = CastChecked<UBaseAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	MageAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	MageAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	MageAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnManaChange.Broadcast(Data.NewValue);
	}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	MageAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxmanaChange.Broadcast(Data.NewValue);
	}
	);
	
	// Like Delegate but it will called That function instead
	Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda([this
		](const FGameplayTagContainer& AssetTags)
	{
		for (const auto& Tag : AssetTags)
		{
			//"Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False

			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			if(Tag.MatchesTag(MessageTag))
			{
				// Row Data
				const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
				MessageWidgetRow.Broadcast(*Row);
			}
		}
	}
	);
	
}
