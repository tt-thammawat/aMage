// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interact/InteractInterface.h"
#include "Inventory/ItemStruct.h"
#include "MainPlayerCharacter.generated.h"



class AProjectile;
class AMainPlayerController;
enum class ETurningInPlace : uint8;
class APickUpEffectActor;
class ABaseEffectActor;
class AMainPlayerState;
/**
 * 
 */
UCLASS()
class AMAGE_API AMainPlayerCharacter : public ABaseCharacter , public IInteractInterface
{
	GENERATED_BODY()
public:
	AMainPlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//CombatInterface
	FORCEINLINE virtual  int32 GetPlayerLevel() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	virtual void BeginPlay() override;

	//Init GAS
	virtual void InitAbilityActorInfo() override;
	void TrySetupHUD(AMainPlayerState* MainPlayerState);

	//TODO : MAy Remove Below
	//Replicated ItemData
	UPROPERTY(ReplicatedUsing=OnRep_ItemDataChange,BlueprintReadOnly)
	FItemData ItemData;
	UFUNCTION()
	void OnRep_ItemDataChange();
	//Interact Interface
	virtual void InteractWithItem(AActor* InteractActor) override;
	//Object Reference
	UPROPERTY(Replicated,BlueprintReadOnly)
	TObjectPtr<AActor> InteractObjectActor;
	//InteractButtonPress
	UFUNCTION()
	virtual void InteractItemButtonPress();
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

public:
	void SetInteractObjectActor(AActor* Actor) {InteractObjectActor = Actor;};



	
};
