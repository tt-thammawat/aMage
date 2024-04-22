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
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	void HighlightActor_Implementation() override;
	void UnHighlightActor_Implementation() override;

	UFUNCTION(NetMulticast,Unreliable)
	void MulticastHighlight();
	UFUNCTION(NetMulticast,Unreliable)
	void MulticastUnHighlight();
	
	UFUNCTION()
	void OnRep_bIsPickup();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Weapon)
	TObjectPtr<USkeletalMeshComponent> ItemEquipMesh;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Weapon)
	FName WeaponTipSocketName;
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Item,meta=(AllowPrivateAccess=true))
	TObjectPtr<UMainAbilitiesItemComponent> MainAbilitiesItemComponent;
	
	UPROPERTY(ReplicatedUsing=OnRep_bIsPickup,EditAnywhere,BlueprintReadWrite,Category=Item,meta=(AllowPrivateAccess=true))
	bool bIsPickup = false;
public:
	USkeletalMeshComponent* GetSkeletalMeshComponent() const {return ItemEquipMesh;};
	FORCEINLINE FName GetWeaponTipSocketName() const {return WeaponTipSocketName;};
	FORCEINLINE bool GetIsPickup() const {return bIsPickup;};

};
