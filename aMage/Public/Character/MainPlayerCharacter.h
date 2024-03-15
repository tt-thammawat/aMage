// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
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
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;
	//CombatInterface
	FORCEINLINE virtual int32 GetCharacterLevel() override;


protected:
	virtual void BeginPlay() override;
	//Start Equip Default Item After Possese
	UFUNCTION(BlueprintImplementableEvent)
	void StartEquipDefaultItem();
	UFUNCTION(Server,Reliable)
	void ServerStartEquipDefaultItem();
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
	
	//CrossHairSpread
	/*Determines The Spread Of The CrossHairs*/
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly,Category = CrossHairs , meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier;
	/*Velocity Component for CrossHairs Spread*/
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = CrossHairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairVelocityFactor;
	/*InAIr Component for CrossHairs Spread*/
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = CrossHairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairInAirFactor;
	/*Shooting Component for CrossHairs Spread*/
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = CrossHairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairShootingFactor;
	
	void CalculateCrossHairSpreadByActor(float DeltaTime);
	
	//Init GAS
	virtual void InitAbilityActorInfo() override;
	void TrySetupHUD(AMainPlayerState* MainPlayerState);
	void BindButtonToCharacter(AMainPlayerController* PlayerController);
	UFUNCTION(BlueprintCallable,Category = Weapon)
	virtual FVector GetCombatSocketLocation_Implementation() override;
	
	//Item Inventory+Equipping
	UFUNCTION(BlueprintCallable,Category = Weapon)
	void AddItemAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AddItemAbilities,UObject* SourceObject);
	UFUNCTION(BlueprintCallable,Category = Weapon)
	void RemoveItemAbilities(const TArray<TSubclassOf<UGameplayAbility>>& RemoveItemAbilities,const TArray<TSubclassOf<UGameplayAbility>>& PotionGrantedAbilities);
	UFUNCTION(BlueprintImplementableEvent,Category = Weapon)
	void OnChangingButtonPressed(int ButtonNumber);
	UFUNCTION(BlueprintImplementableEvent,Category = Weapon)
	void OnDropButtonPressed();
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
	
	//Request Ability After Get Casting Rune Spell
	UPROPERTY(Replicated)
	FGameplayAbilitySpecHandle AbilityHandle;
	virtual void MatchRuneSpellTags(TArray<FGameplayTag> RuneTags) override;
	UFUNCTION(Server,Reliable)
	void ServerRequestAbilityActivation(const TArray<FGameplayTag>& RuneTags);
	void ProcessAbilityRequest(const TArray<FGameplayTag>& RuneTags);
	
	//Clear Rune Spell
	void ClearRuneSpell_Implementation() override;
	UFUNCTION(Server,Reliable)
	void ServerRequestClearRuneSpell();
	void ProcessClearRuneSpellRequest();
public:
	
	//FireSpell
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite,Category = CrossHairs , meta = (AllowPrivateAccess = "true"))
	bool bFiringSpell = false;
	
	UFUNCTION(BlueprintCallable,Category=Interact)
	void SetInteractObjectActor(AActor* Actor);
	UFUNCTION(BlueprintCallable,Category=Interact)
	void ClearInteractObjectActor(AActor* Actor);

	UFUNCTION()
	void SetIsAiming(bool IsAiming) {bIsAiming = IsAiming;};
	FORCEINLINE bool GetIsAiming() const {return bIsAiming;};
	FORCEINLINE float GetAOYaw() const {return AO_Yaw;};
	FORCEINLINE float GetAOPitch() const {return AO_Pitch;};

	//CrossHair
	
};
