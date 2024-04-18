// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/State_Notify.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void UState_Notify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	{
		if (SoundToPlay)
		{
			AudioComponent = UGameplayStatics::SpawnSoundAttached
			(
			SoundToPlay, MeshComp,
			NAME_None,
			FVector(ForceInit),
			EAttachLocation::KeepRelativeOffset,
			true
			);
		}
	}
}

void UState_Notify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent = NULL;
	}
}
