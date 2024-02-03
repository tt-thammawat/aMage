// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BaseEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/EffectDataAsset.h"

ABaseEffectActor::ABaseEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void ABaseEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

//Apply Effect Here
void ABaseEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass,
	float& ActorLevel)
{

	//Magic Function To Get ASC can get from Interface and Component
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(TargetASC == nullptr) return;

	checkf(GameplayEffectClass,TEXT("NO GamplayEffectClass,EffectActor"));
	//Create Context For Effect
	FGameplayEffectContextHandle EffectContextHandle =  TargetASC->MakeEffectContext();
	//Sets the object this effect was created from
	EffectContextHandle.AddSourceObject(this);
	//Allows blueprints to generate a GameplayEffectSpec once and then reference it by handle, to apply it multiple times/multiple targets.

	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass,ActorLevel,EffectContextHandle);
	//Pointer Need To be Dereference and SmartPointer Need To Dereference By Get
	const FActiveGameplayEffectHandle ActiveGameplayEffectHandle= TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		
	if(EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite)
	{
		for (const auto& EffectData : EffectDataAssets)
		{
			if(EffectData->InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap || EffectData->InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnOverlap)
			{
				//Key Value Pair
				ActiveEffectHandles.Add(ActiveGameplayEffectHandle,TargetASC);
			}
		}
	}
}


void ABaseEffectActor::OnOverlap(AActor* TargetActor)
{
	//Case InstantEffect
	for (const auto& EffectData : EffectDataAssets)
	{
		if(EffectData->InstantGameplayEffectClass)
		{
			switch (EffectData->InstantEffectApplicationPolicy)
			{
			case EEffectApplicationPolicy::ApplyOnOverlap:
				if(EffectData->InstantGameplayEffectClass)
				{
					ApplyEffectToTarget(TargetActor,EffectData->InstantGameplayEffectClass,EffectData->ActorLevel);
				}
				break;
			default:
				break;
			}
		}
		//Case DurationEffect
		if(EffectData->DurationGameplayEffectClass)
		{
			switch (EffectData->DurationEffectApplicationPolicy)
			{
			case EEffectApplicationPolicy::ApplyOnOverlap:
				if(EffectData->DurationGameplayEffectClass)
				{
					ApplyEffectToTarget(TargetActor,EffectData->DurationGameplayEffectClass,EffectData->ActorLevel);
				}
				break;
			default:
				break;
			}
		}

		//Case InfiniteEffect
		if(EffectData->InfiniteGameplayEffectClass)
		{
			switch (EffectData->InfiniteEffectApplicationPolicy)
			{
			case EEffectApplicationPolicy::ApplyOnOverlap:
					if(EffectData->InfiniteGameplayEffectClass)
					{
						ApplyEffectToTarget(TargetActor,EffectData->InfiniteGameplayEffectClass,EffectData->ActorLevel);
					}
					break;
				default:
					break;
			}
		}
	}

}

void ABaseEffectActor::OnEndOverlap(AActor* TargetActor)
{
	//Case InfiniteEffect
	for (const auto& EffectData : EffectDataAssets)
	{
		switch (EffectData->InfiniteEffectRemovePolicy)
		{
		case EEffectRemovePolicy::RemoveOnEndOverlap:
			RemoveSingleEffect(TargetActor);
			break;
		default:
			break;
		}
	}
}

void ABaseEffectActor::RemoveSingleEffect(AActor* RemoveEffectTarget)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(RemoveEffectTarget);

	if(!IsValid(TargetASC)) return;
		
	// Initialize array to keep track which effect handles to remove
	TArray<FActiveGameplayEffectHandle> HandleToRemove;
	for (const auto& HandlePair : ActiveEffectHandles)
	{
		// Check if the current effect handle's ASC matches TargetASC
		if(TargetASC == HandlePair.Value)
		{
			// Remove the active gameplay effect from TargetASC
			TargetASC->RemoveActiveGameplayEffect(HandlePair.Key,1);
			// Add this handle to the list of handles to remove from the map
			HandleToRemove.Add(HandlePair.Key);
		}
	}

	// Remove the collected handles from the ActiveEffectHandles map
	for(const auto& Handle : HandleToRemove)
	{
		ActiveEffectHandles.FindAndRemoveChecked(Handle);
	}
}


