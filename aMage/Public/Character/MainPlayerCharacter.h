// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interact/CastingInterface.h"
#include "MainPlayerCharacter.generated.h"

class UAmage_EquipmentManager;
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
class AMAGE_API AMainPlayerCharacter : public ABaseCharacter,public  ICastingInterface
{
	GENERATED_BODY()
public:
	AMainPlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	//CombatInterface
	FORCEINLINE virtual int32 GetCharacterLevel() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void BeginPlay() override;
	//Aiming
	UPROPERTY(Replicated,VisibleAnywhere,BlueprintReadOnly)
	bool bIsAiming;
	void AimOffset(float DeltaTime);
	UPROPERTY(BlueprintReadOnly)
	float AO_Yaw;
	float InterpAO_Yaw;
	UPROPERTY(BlueprintReadOnly)
	float AO_Pitch;
	FRotator StartingAimRotation;
	
	//Init GAS
	virtual void InitAbilityActorInfo() override;
	void TrySetupHUD(AMainPlayerState* MainPlayerState);
	void BindButtonToCharacter(AMainPlayerController* PlayerController);
	UFUNCTION(BlueprintCallable,Category = Weapon)
	virtual FVector GetCombatSocketLocation() override;
	
	//Item Inventory+Equipping
	UFUNCTION(BlueprintCallable,Category = Weapon)
	void AddItemAbilities(TSubclassOf<UGameplayAbility> AddItemAbility);
	UFUNCTION(BlueprintCallable,Category = Weapon)
	void RemoveItemAbilities(TSubclassOf<UGameplayAbility> RemoveItemAbility);
	UFUNCTION(BlueprintImplementableEvent,Category = Weapon)
	void OnChangingButtonPressed(int ButtonNumber);
	
	//Object Reference
	UPROPERTY(ReplicatedUsing=OnRep_InteractObjectActor,BlueprintReadOnly,Category=Interact)
	TObjectPtr<AActor> InteractObjectActor;
	UFUNCTION()
	void OnRep_InteractObjectActor(AActor* OldInteractObject);
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=AGR)
	TObjectPtr<UAmage_EquipmentManager> PlayerEquipmentManager;
	
	//InteractButtonPress Delegate From PlayerController
	UFUNCTION()
	virtual void InteractItemButtonPress();
	UFUNCTION(Server, Reliable)
	void ServerInteractButtonPressed();

	//Interface Request Ability After Get Casting Rune Spell
	UPROPERTY(Replicated)
	TArray<FGameplayTag> RefRuneTags;
	virtual void MatchRuneSpellTags(TArray<FGameplayTag> RuneTags) override;
	UFUNCTION(Server,Reliable)
	void ServerRequestAbilityActivation(const TArray<FGameplayTag>& RuneTags);
	void ProcessAbilityRequest(const TArray<FGameplayTag>& RuneTags);

public:
	UFUNCTION(BlueprintCallable,Category=Interact)
	void SetInteractObjectActor(AActor* Actor);
	UFUNCTION(BlueprintCallable,Category=Interact)
	void ClearInteractObjectActor(AActor* Actor);

	UFUNCTION()
	void SetIsAiming(bool IsAiming) {bIsAiming = IsAiming;};
	FORCEINLINE bool GetIsAiming() const {return bIsAiming;};
	FORCEINLINE float GetAOYaw() const {return AO_Yaw;};
	FORCEINLINE float GetAOPitch() const {return AO_Pitch;};

};
