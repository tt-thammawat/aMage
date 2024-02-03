// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;


}

//return this ASC
UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;

}

//Override by derived
void ABaseCharacter::InitAbilityActorInfo()
{
	
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}




