// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Inventory/ItemStruct.h"
#include "MainPlayerCharacter.generated.h"


class UAGR_EquipmentManager;
class UAGR_InventoryManager;
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
class AMAGE_API AMainPlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AMainPlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//CombatInterface
	FORCEINLINE virtual  int32 GetCharacterLevel() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	virtual void BeginPlay() override;

	//Init GAS
	virtual void InitAbilityActorInfo() override;
	void TrySetupHUD(AMainPlayerState* MainPlayerState);
	
	
	//Item Inventory+Equipping
	void AddItemAbilities() const;
	void RemoveItemAbilities() const;
	//TODO : MAy Remove Below
	//Replicated ItemData
	UPROPERTY(ReplicatedUsing=OnRep_ItemDataChange,BlueprintReadOnly)
	FItemData ItemData;
	UFUNCTION()
	void OnRep_ItemDataChange() const;
	//End ToDo
	
	//Object Reference
	UPROPERTY(Replicated,BlueprintReadOnly,Category=Interact)
	TObjectPtr<AActor> InteractObjectActor;
	//Inventory
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=AGR)
	TObjectPtr<UAGR_InventoryManager> InventoryManager;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=AGR)
	TObjectPtr<UAGR_EquipmentManager> EquipmentManager;
	
	//InteractButtonPress Delegate From PlayerController
	UFUNCTION()
	virtual void InteractItemButtonPress();
	UFUNCTION(Server, Reliable)
	void ServerInteractButtonPressed();

public:
	UFUNCTION(BlueprintCallable,Category=Interact)
	void SetInteractObjectActor(AActor* Actor);
	UFUNCTION(BlueprintCallable,Category=Interact)
	void ClearInteractObjectActor(AActor* Actor);


	
};
