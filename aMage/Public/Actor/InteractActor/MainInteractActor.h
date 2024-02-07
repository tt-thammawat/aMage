// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact/InteractInterface.h"
#include "MainInteractActor.generated.h"

class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorOverlapSignature,AActor*,Actor);


UCLASS()
class AMAGE_API AMainInteractActor : public AActor,public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AMainInteractActor();
	
protected:
	virtual void BeginPlay() override;

	//InteractInterface
	virtual void InteractWithItem() override;
	virtual void ShowInteractDetail() override;
	virtual void HideInteractDetail() override;
	
	UPROPERTY(BlueprintAssignable,Category=Overlap)
	FOnActorOverlapSignature OnActorOverlap;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item Widget")
	TObjectPtr<UWidgetComponent> InteractWidget;

public:	

};
