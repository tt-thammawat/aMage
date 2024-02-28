// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/InterpolateFOV.h"

#include "Camera/CameraComponent.h"


UInterpolateFOV* UInterpolateFOV::InterpolateFOV(UGameplayAbility* OwningAbility, UCameraComponent* CameraComponent, float InitialFOV, float TargetFOV, float Duration)
{
	UInterpolateFOV* MyTask = NewAbilityTask<UInterpolateFOV>(OwningAbility);
	MyTask->CameraComponent = CameraComponent;
	MyTask->InitialFOV = InitialFOV;
	MyTask->TargetFOV = TargetFOV;
	MyTask->InterpolationDuration = Duration;
	return MyTask;
}

void UInterpolateFOV::Activate()
{
	Super::Activate();
	StartTime = GetWorld()->GetTimeSeconds();

	// Setup a timer to repeatedly call InterpolationTick
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInterpolateFOV::InterpolationTick, 0.01f, true);
}

void UInterpolateFOV::OnDestroy(bool bInOwnerFinished)
{
	OnInterpolationCompleted.Broadcast();
	Super::OnDestroy(bInOwnerFinished);
}

void UInterpolateFOV::InterpolationTick()
{
	if (!CameraComponent) {
		EndTask();
		return;
	}

	float CurrentTime = GetWorld()->GetTimeSeconds() - StartTime;
	float Alpha = FMath::Clamp(CurrentTime / InterpolationDuration, 0.0f, 1.0f);
	float NewFOV = FMath::Lerp(InitialFOV, TargetFOV, Alpha);

	CameraComponent->SetFieldOfView(NewFOV);

	if (Alpha >= 1.0f) {
		// Interpolation complete, end the task
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		EndTask();

	}
}
