// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/TargetDataUnderLineTrace.h"
#include "AbilitySystemComponent.h"


UTargetDataUnderLineTrace* UTargetDataUnderLineTrace::TargetDataUnderLineTrace(UGameplayAbility* OwningAbility,float DefaultDistance,bool bIsHoming)
{
	UTargetDataUnderLineTrace* MyTask = NewAbilityTask<UTargetDataUnderLineTrace>(OwningAbility);
	MyTask->DefaultDistance = DefaultDistance;
	MyTask->bIsHoming = bIsHoming;
	return MyTask;
}

void UTargetDataUnderLineTrace::PerformLineTrace()
{
	if (!Ability) return;

	FHitResult TraceHitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Ability->GetCurrentActorInfo()->AvatarActor.Get());
	FVector2D ViewPortSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}
	//Center Of The Viewport Vector
	const FVector2D CrossHairLocation(ViewPortSize.X/2,ViewPortSize.Y/2);
	
	APlayerController* PC = Cast<APlayerController>(Ability->GetCurrentActorInfo()->PlayerController);
	bool bHit  =(PC->GetHitResultAtScreenPosition(CrossHairLocation,ECollisionChannel::ECC_Visibility,Params,TraceHitResult));
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	
	//set this if homing
	 if(bHit && bIsHoming)
	 {
	 	Data->HitResult = TraceHitResult;
	 }
	else if (!bHit || !bIsHoming)
	{
		FVector WorldLocation, WorldDirection;
		// Convert the crosshair screen position to a world space direction
		if (PC->DeprojectScreenPositionToWorld(CrossHairLocation.X, CrossHairLocation.Y, WorldLocation, WorldDirection))
		{
			FVector DefaultImpactPoint = WorldLocation + (WorldDirection * DefaultDistance);
			
			// Prepare the fake hit result
			TraceHitResult = FHitResult(ForceInit);
			TraceHitResult.ImpactPoint = DefaultImpactPoint;
			TraceHitResult.Location = DefaultImpactPoint; // Use if ImpactPoint is not available

			Data->HitResult = TraceHitResult;
		}
	}
	
	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);

	//ServerSetReplicatedTargetData from AbilitySystemComponent
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		
		AbilitySystemComponent->ScopedPredictionKey);

	//Prevent Broadcast when we  was blocked
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		OnLineTraceComplete.Broadcast(DataHandle);
	}
}

void UTargetDataUnderLineTrace::Activate()
{
	const bool bIslocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if(bIslocallyControlled)
	{
		PerformLineTrace();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle=GetAbilitySpecHandle();
		const FPredictionKey ActivatePredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle,ActivatePredictionKey).AddUObject(this,&ThisClass::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate=	AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle,ActivatePredictionKey);
		//True TargetData already send To Server / false Server Still not Get TargetDAta :: WAit for Player To Send That Data
		if(!bCalledDelegate)
		{
			//WaitingOnPlayerData
			SetWaitingOnRemotePlayerData();
		}
	}
}



void UTargetDataUnderLineTrace::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	//clear CachedData once the server receive target delegate
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());

	if(ShouldBroadcastAbilityTaskDelegates())
	{
		OnLineTraceComplete.Broadcast(DataHandle);
	}
}
