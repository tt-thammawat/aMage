// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_CheckIsDead.generated.h"

/**
 * 
 */
//TODO: Check Dead Not In Tick But In When HP = 0;
UCLASS()
class AMAGE_API UBTService_CheckIsDead : public UBTService_BlueprintBase
{
	GENERATED_BODY()
protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
