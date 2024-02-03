// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/BaseCharacter.h"
#include "Interact/TargetInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "BaseEnemy.generated.h"

enum class ECharacterClass : uint8;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AMAGE_API ABaseEnemy : public ABaseCharacter, public ITargetInterface
{
	GENERATED_BODY()
public:
	ABaseEnemy();
	
	// Enemy Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	// End Enemy Interface

	virtual int32 GetPlayerLevel() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitDefaultAttributes() const override;

	//Health Bar
	void BroadCastHealthValue();
	void BindCallBackHealthValue();
	UPROPERTY(BlueprintAssignable)
	FOnAttributesChangedSignature OnEnemyHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributesChangedSignature OnEnemyMaxHealthChanged;
	
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "Character Class Defaults",meta=(AllowPrivateAccess=true))
	int32 Level=1;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "Character Class Defaults",meta=(AllowPrivateAccess=true))
	ECharacterClass CharacterClass=ECharacterClass::Warrior;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	TObjectPtr<UWidgetComponent> HealthBar;
};
