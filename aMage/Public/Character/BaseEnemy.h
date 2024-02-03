// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interact/TargetInterface.h"
#include "BaseEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AMAGE_API ABaseEnemy : public ABaseCharacter, public ITargetInterface
{
	GENERATED_BODY()
public:
	ABaseEnemy();
	
	// Enemy Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	// End Enemy Interface
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void InitAbilityActorInfo() override;


private:
};
