// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootStepsComponent.generated.h"

enum class EFoot : uint8;

UENUM(BlueprintType)
enum class EFoot : uint8
{
	ELeft UMETA(DisplayName = "Left"),
	ERight UMETA(DisplayName = "Right"),
	EMax

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AMAGE_API UFootStepsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFootStepsComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	FName LeftFootSocketName = TEXT("foot_l");
	
	UPROPERTY(EditDefaultsOnly)
	FName RightFootSocketName = TEXT("foot_r");
	
	
public:	
	void HandleFootStep(EFoot Foot);
		
};
