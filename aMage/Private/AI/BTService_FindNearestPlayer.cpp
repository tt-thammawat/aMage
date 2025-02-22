// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindNearestPlayer.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "GameplayTagsSingleton.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn =  AIOwner->GetPawn();

	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy"): FName("Player");

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn,TargetTag,ActorsWithTag);
	
	AActor* ClosestActor = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();
	
	for(AActor* Actor : ActorsWithTag)
	{
		if(!IsValid(Actor) || !IsValid(OwningPawn))
		{
			continue;
		}
		
		IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Actor);
		if(AbilitySystemInterface)
		{
			UAbilitySystemComponent* ASC = AbilitySystemInterface->GetAbilitySystemComponent();
			
			if(ASC && ASC->HasMatchingGameplayTag(FMainGameplayTags::Get().Effects_Buff_Death))
			{
				continue;
			}
		}
		
		if(IsValid(Actor) && IsValid(OwningPawn))
		{
			const float Distance = OwningPawn->GetDistanceTo(Actor);
			if(Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = Actor;
			}
		}
	}
	
	UBTFunctionLibrary::SetBlackboardValueAsObject(this,TargetToFollowSelector,ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this,DistanceToTargetSelector,ClosestDistance);
}
