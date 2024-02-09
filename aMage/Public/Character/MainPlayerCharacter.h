// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
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

	UFUNCTION(BlueprintCallable,Category = Weapon)
	virtual FVector GetCombatSocketLocation() override;

	
	//Item Inventory+Equipping
	UFUNCTION(BlueprintCallable,Category = Weapon)
	void AddItemAbilities(TSubclassOf<UGameplayAbility>& AddItemAbility);
	UFUNCTION(BlueprintCallable,Category = Weapon)
	void RemoveItemAbilities(TSubclassOf<UGameplayAbility>& RemoveItemAbility);
	
	//Object Reference
	UPROPERTY(ReplicatedUsing=OnRep_InteractObjectActor,BlueprintReadOnly,Category=Interact)
	TObjectPtr<AActor> InteractObjectActor;
	UFUNCTION()
	void OnRep_InteractObjectActor(AActor* OldInteractObject);
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=AGR)
	TObjectPtr<UAGR_EquipmentManager> PlayerEquipmentManager;
	
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
