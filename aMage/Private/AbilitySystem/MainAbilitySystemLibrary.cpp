// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MainAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/MainPlayerState.h"
#include "Gamemode/MainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/MainPlayerHUD.h"
#include "UI/WidgetController/BaseWidgetController.h"

UOverlayWidgetController* UMainAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if(APlayerController* PlayerController = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if(AMainPlayerHUD* MainPlayerHUD = Cast<AMainPlayerHUD>(PlayerController->GetHUD()))
		{
			if(AMainPlayerState* MainPlayerState = PlayerController->GetPlayerState<AMainPlayerState>())
			{
				UAbilitySystemComponent* ASC = MainPlayerState->GetAbilitySystemComponent();
				UAttributeSet* AS = MainPlayerState->GetPlayerStateAttributeSet();
				const FWidgetControllerParams WidgetControllerParams(PlayerController,MainPlayerState,ASC,AS);
				return MainPlayerHUD->GetOverlayWidgetController(WidgetControllerParams);;
			}
		}
	}
	return nullptr;
	
}

void UMainAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* TargetASC)
{
	const AMainGameMode* MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(MainGameMode == nullptr) return;

	//Need To Set Attributes To The Avatar in case of multiplayer
	AActor* AvatarActor = TargetASC->GetAvatarActor();

		//--PrimaryAttributes-------------------------------------------------------------------------------------------------------//

	//Add SourceObject to correctly apply effect to
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = TargetASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	
	// Access to UCharacterClassInfo functions
const	FCharacterClassDefaultInfo ClassDefaultInfo = MainGameMode->CharacterClassDefaultInfo->GetClassDefaultInfo(CharacterClass);

	//Make GameplayEffectSpecHandle To Apply Attributes
 const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle=	TargetASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes,Level,PrimaryAttributesContextHandle);
	
	//Apply GamePlayEffect To Spawn
	TargetASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());
	//---------------------------------------------------------------------------------------------------------------------------------//
	//--SecondaryAttributes-------------------------------------------------------------------------------------------------------//

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = TargetASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle =	TargetASC->MakeOutgoingSpec(MainGameMode->CharacterClassDefaultInfo->SecondaryAttributes,Level,SecondaryAttributesContextHandle);
	
	TargetASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	//---------------------------------------------------------------------------------------------------------------------------------//
	//--VitalAttributes-------------------------------------------------------------------------------------------------------//
	FGameplayEffectContextHandle VitalAttributeContextHandle = TargetASC->MakeEffectContext();
	VitalAttributeContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle VitalAttributesSpecHandle = TargetASC->MakeOutgoingSpec(MainGameMode->CharacterClassDefaultInfo->VitalAttributes,Level,VitalAttributeContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
	//---------------------------------------------------------------------------------------------------------------------------------//

}

FString UMainAbilitySystemLibrary::GenerateUniqueKeyFromFName(FName Name)
{
	FString NameStr = Name.ToString();

	FDateTime Now = FDateTime::UtcNow();

	FGuid Guid = FGuid::NewGuid();
	FString GuidStr = Guid.ToString();

	// Combine the FName string, timestamp, and GUID to form a unique key
	FString UniqueKey = FString::Printf(TEXT("%s_%s_%s"), *NameStr, *Now.ToString(), *GuidStr);
	
	return UniqueKey;


}

