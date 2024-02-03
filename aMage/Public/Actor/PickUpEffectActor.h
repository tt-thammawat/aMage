// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEffectActor.h"
#include "Character/Inventory/ItemStruct.h"
#include "PickUpEffectActor.generated.h"


class AMainPlayerCharacter;

UCLASS()
class AMAGE_API APickUpEffectActor : public ABaseEffectActor
{
	GENERATED_BODY()

public:
	APickUpEffectActor();
	//PlayerPickup
	virtual void DestroyAfterPickUp(AActor* TargetActor);

protected:
	virtual void BeginPlay() override;

	//Interact With Actor
	UFUNCTION(BlueprintCallable)
	virtual void OnInteractWithActor(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	virtual void OnEndInteractWithActor(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable)
	void ShowDetail();
	UFUNCTION(BlueprintCallable)
	void UnShowDetail();
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item Widget")
	TObjectPtr<UWidgetComponent> PickUpWidget;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item skeletonMesh")
	TObjectPtr<USkeletalMeshComponent> PickUpSkeletalMeshComponent;
	//Item Data Set IT In Blueprint
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item Properties")
	FItemData ItemData;

public:
	FItemData GetItemData() const {return ItemData;};
	

	
	



	
};
