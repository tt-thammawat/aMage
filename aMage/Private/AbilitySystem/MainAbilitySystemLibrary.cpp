// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MainAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsSingleton.h"
#include "MainAbilityTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/MainPlayerState.h"
#include "Gamemode/MainGameMode.h"
#include "Interact/ICombatInterface.h"
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

	//--PrimaryAttributes
	//Add SourceObject to correctly apply effect to
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = TargetASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	
	// Access to UCharacterClassInfo functions
	const FCharacterClassDefaultInfo ClassDefaultInfo = MainGameMode->CharacterClassDefaultInfo->GetClassDefaultInfo(CharacterClass);

	//Make GameplayEffectSpecHandle To Apply Attributes
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle=	TargetASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes,Level,PrimaryAttributesContextHandle);
	
	//Apply GamePlayEffect To Spawn
	TargetASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());
	
	//--SecondaryAttributes
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = TargetASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle =	TargetASC->MakeOutgoingSpec(MainGameMode->CharacterClassDefaultInfo->SecondaryAttributes,Level,SecondaryAttributesContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	//--VitalAttributes
	FGameplayEffectContextHandle VitalAttributeContextHandle = TargetASC->MakeEffectContext();
	VitalAttributeContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle VitalAttributesSpecHandle = TargetASC->MakeOutgoingSpec(MainGameMode->CharacterClassDefaultInfo->VitalAttributes,Level,VitalAttributeContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

UCharacterClassInfo* UMainAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	//Access TO ACrystalGamemodeBase

	const AMainGameMode* MainGameMode=	Cast<AMainGameMode>(UGameplayStatics::GetGameMode	(WorldContextObject));
	if(MainGameMode == nullptr) return nullptr;

	UCharacterClassInfo* CharacterClassInfo = MainGameMode->CharacterClassDefaultInfo;
	return CharacterClassInfo;
}

void UMainAbilitySystemLibrary::GiveStartUpAbilities(const UObject* WorldContextObject,
	UAbilitySystemComponent* TargetASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec=	FGameplayAbilitySpec(AbilityClass,1);
		TargetASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		IICombatInterface* CombatInterface= Cast<IICombatInterface>(TargetASC->GetAvatarActor());
		if(CombatInterface)
		{
			FGameplayAbilitySpec AbilitySpec=	FGameplayAbilitySpec(AbilityClass,CombatInterface->GetCharacterLevel());
			TargetASC->GiveAbility(AbilitySpec);
		}
	}
}

bool UMainAbilitySystemLibrary::IsFireDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get());
	if(MainGameplayEffectContext)
	{
		return MainGameplayEffectContext->IsFireDamage();
	}
	return false;
}

bool UMainAbilitySystemLibrary::IsLightningDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get());
	if(MainGameplayEffectContext)
	{
		return MainGameplayEffectContext->IsLightningDamage();
	}
	return false;
}

bool UMainAbilitySystemLibrary::IsIceDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get());
	if(MainGameplayEffectContext)
	{
		return MainGameplayEffectContext->IsIceDamage();
	}
	return false;
}

bool UMainAbilitySystemLibrary::IsPhysicalDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get());
	if(MainGameplayEffectContext)
	{
		return MainGameplayEffectContext->IsPhysicalDamage();
	}
	return false;

}

void UMainAbilitySystemLibrary::SetIsFireDamage(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsFireDamage)
{
	FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get());
	if(MainGameplayEffectContext)
	{
		MainGameplayEffectContext->SetIsFireDamage(bInIsFireDamage);
	}
}

void UMainAbilitySystemLibrary::SetIsLightningDamage(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsLightningDamage)
{
	FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get());
	if(MainGameplayEffectContext)
	{
		MainGameplayEffectContext->SetIsLightningDamage(bInIsLightningDamage);
	}
}

void UMainAbilitySystemLibrary::SetIsIceDamage(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsIceDamage)
{
	FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get());
	if(MainGameplayEffectContext)
	{
		MainGameplayEffectContext->SetIsIceDamage(bInIsIceDamage);
	}
}

void UMainAbilitySystemLibrary::SetIsPhysicalDamage(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsPhysicalDamage)
{
	FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get());
	if(MainGameplayEffectContext)
	{
		MainGameplayEffectContext->SetIsPhysicalDamage(bInIsPhysicalDamage);
	}
}

void UMainAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                           TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                           const FVector& SphereOrigin)
{
	//Do Query
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	TArray<FOverlapResult> Overlaps;
	if(const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for( FOverlapResult& OverlapResult : Overlaps)
		{
			if(OverlapResult.GetActor()->Implements<UICombatInterface>() && !IICombatInterface::Execute_IsDead(OverlapResult.GetActor()))
			{
				//Prevent Duplication
				OutOverlappingActors.AddUnique(IICombatInterface::Execute_GetAvatar(OverlapResult.GetActor()));
			}
		}
	}
}

void UMainAbilitySystemLibrary::GetDeadPlayersWithinRadius(const UObject* WorldContextObject,
															TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
															const FVector& SphereOrigin)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;

	FCollisionQueryParams SphereParams(FName(TEXT("DeadPlayersQuery")), true);
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, 
									 FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), 
									 FCollisionShape::MakeSphere(Radius), SphereParams);

	FGameplayTag DeathTag = FMainGameplayTags::Get().Effects_Buff_Death;

	for (const FOverlapResult& OverlapResult : Overlaps)
	{
		AActor* OverlappedActor = OverlapResult.GetActor();
		if (!OverlappedActor || ActorsToIgnore.Contains(OverlappedActor) || !OverlappedActor->ActorHasTag("Player"))
		{
			continue;
		}

		IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OverlappedActor);
		UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface ? AbilitySystemInterface->GetAbilitySystemComponent() : nullptr;
		if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(DeathTag))
		{
			OutOverlappingActors.AddUnique(OverlappedActor);
		}
	}
}

void UMainAbilitySystemLibrary::GetLiveActorsInBeam(const UObject* WorldContextObject,
													 TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, 
													 const FVector& BeamStart, const FVector& BeamEnd, const FVector& BeamSize)
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);

	FVector BeamDirection = (BeamEnd - BeamStart).GetSafeNormal();
	FRotator BeamRotator = BeamDirection.Rotation();
	FQuat BeamRotation = FQuat(BeamRotator);
	TArray<FHitResult> HitResults;
	
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->SweepMultiByObjectType(HitResults, BeamStart, BeamEnd, BeamRotation, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeBox(BeamSize), QueryParams);
		
		for (const FHitResult& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor && HitActor->Implements<UICombatInterface>() && !IICombatInterface::Execute_IsDead(HitActor))
			{
				OutOverlappingActors.AddUnique(IICombatInterface::Execute_GetAvatar(HitActor));
			}
		}
	}
}

bool UMainAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	if (!FirstActor || !SecondActor)
	{
		return true;
	}
	
	//const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothAreEnemies;
	
	return !bFriends;
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

