// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainGameMode.generated.h"

class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class AMAGE_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassDefaultInfo;
};
