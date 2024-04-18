// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/Animnotify_Step.h"

#include "ActorComponents/FootStepsComponent.h"
#include "Character/BaseCharacter.h"

void UAnimnotify_Step::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	check(MeshComp);
	ABaseCharacter* Character = MeshComp ? Cast<ABaseCharacter>(MeshComp->GetOwner()):nullptr;
	if(Character)
	{
		UFootStepsComponent* FootStepsComponent = 	Character->GetFootStepsComponent();
		if(FootStepsComponent)
		{
			FootStepsComponent->HandleFootStep(Foot);
		}
	}
}
