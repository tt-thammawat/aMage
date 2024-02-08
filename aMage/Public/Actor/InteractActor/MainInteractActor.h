// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact/InteractInterface.h"
#include "MainInteractActor.generated.h"

class UWidgetComponent;



UCLASS()
class AMAGE_API AMainInteractActor : public AActor,public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AMainInteractActor();
	
protected:
	virtual void BeginPlay() override;

	
	
	//InteractInterface
	virtual void InteractWithItem(AActor* Actor) override;
	virtual void ShowInteractDetail() override;
	virtual void HideInteractDetail() override;
	

protected:
	UFUNCTION(BlueprintImplementableEvent,Category=Interact)
	void GetInteractActor(AActor* Actor);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Default")
	TObjectPtr<UWidgetComponent> InteractWidget;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Default")
	TObjectPtr<USceneComponent> CustomRootComponent;
public:	

};
