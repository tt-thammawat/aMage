// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MainAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "MainAbilityTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/MainPlayerState.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Gamemode/MainGameMode.h"
#include "Interact/ICombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/MainPlayerHUD.h"
#include "Engine/TextureRenderTarget2D.h"
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

bool UMainAbilitySystemLibrary::CaptureMeshWithCameraAndBounds(USceneCaptureComponent2D* SceneCaptureComponent, 
	float SphereRadius, float BoundingBoxExtentY, float BoundingBoxExtentZ)
{
	if (!SceneCaptureComponent)
	{
		return false;
	}
	
	float NewCameraX = BoundingBoxExtentY + BoundingBoxExtentZ+SphereRadius;

	FVector CurrentLocation = SceneCaptureComponent->GetRelativeLocation();

	FVector NewCameraLocation = FVector(NewCameraX, CurrentLocation.Y, CurrentLocation.Z);

	SceneCaptureComponent->SetRelativeLocation(-NewCameraLocation);

	// Capture the scene.
	SceneCaptureComponent->CaptureScene();

	return true;
}

void UMainAbilitySystemLibrary::CreateIcon(UTextureRenderTarget2D* TextureRenderTarget, UTexture2D*& OutputIcon)
{
	if (!TextureRenderTarget)
	{
		OutputIcon = nullptr;
		return;
	}

	// Create a new Texture2D to store the RenderTarget content.
	UTexture2D* Texture = UTexture2D::CreateTransient(TextureRenderTarget->SizeX, TextureRenderTarget->SizeY, PF_B8G8R8A8);

#if WITH_EDITORONLY_DATA
	Texture->MipGenSettings = TMGS_NoMipmaps;
#endif

	// Read the pixels from the RenderTarget and store them in a FColor array.
	TArray<FColor> SurfData;
	FRenderTarget* RenderTargetResource = TextureRenderTarget->GameThread_GetRenderTargetResource();

	if (RenderTargetResource)
	{
		RenderTargetResource->ReadPixels(SurfData);
	}

	// Apply alpha threshold operation: Set the alpha to 0 for black pixels, leave it unchanged for others.
	for (FColor& Pixel : SurfData)
	{
		if (Pixel.R == 0 && Pixel.G == 0 && Pixel.B == 0) // Check if the pixel is black
		{
			Pixel.A = 0; // Set alpha to 0 for black pixels
		}
	}
	// Lock and copy the data between the textures.
	void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	const int32 TextureDataSize = SurfData.Num() * sizeof(FColor);
	FMemory::Memcpy(TextureData, SurfData.GetData(), TextureDataSize);
	Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

	// Apply Texture changes to GPU memory.
	Texture->UpdateResource();

	// Set the output icon.
	OutputIcon = Texture;
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

