// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckIsDead.h"

#include "AIController.h"
#include "AI/MainAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BaseCharacter.h"

void UBTService_CheckIsDead::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	AMainAIController* MainAIController = Cast<AMainAIController>(AIController);
	APawn* OwningPawn =  AIOwner->GetPawn();
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(AIController->GetPawn());

	if(BaseCharacter)
		{
			bool bIsDead = BaseCharacter->GetIsDead();
			MainAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsCharacterDead"), bIsDead);
		}
	
}
