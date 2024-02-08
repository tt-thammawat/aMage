// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/InteractActor/MainInteractActor.h"
#include "MainItemInteractActor.generated.h"

class UMainAbilitiesItemComponent;
/**
 * 
 */
UCLASS()
class AMAGE_API AMainItemInteractActor : public AMainInteractActor
{
	GENERATED_BODY()
public:
	AMainItemInteractActor();
protected:
	virtual void InteractWithItem(AActor* Actor) override;

	UFUNCTION(BlueprintImplementableEvent,Category=Interact)
	void GetInteractActor(AActor* Actor);

private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Item,meta=(AllowPrivateAccess=true))
	TObjectPtr<UMainAbilitiesItemComponent> AbilitiesItemComponent;

};
