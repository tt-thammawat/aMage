// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/BaseCharacter.h"
#include "Interact/TargetInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "BaseEnemy.generated.h"

class UBehaviorTree;
class AMainAIController;
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
	virtual void PossessedBy(AController* NewController) override;
	
	// Enemy Interface
	void HighlightActor_Implementation() override;
	void UnHighlightActor_Implementation() override;
	// End Enemy Interface

	//CombatInterface
	virtual int32 GetCharacterLevel() override;
	virtual void Die() override;
	//EndCombatInterface

	//ITargetInterface
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	//End ITargetInterface

	void HitReactTagChanged(const FGameplayTag CallBackTag , int32 NewCount);

	//boolean Check for Behavior Tree
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Combat")
	bool bHitReacting=false;

	//boolean Check for spell Activation
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Combat")
	bool bGotHit=false;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite,Category="Combat")
	TObjectPtr<AActor> CombatTarget;
	
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

	
	UPROPERTY(EditAnywhere,Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY()
	TObjectPtr<AMainAIController> MainAIController;
	
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "Character Class Defaults",meta=(AllowPrivateAccess=true))
	int32 Level=1;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "Character Class Defaults",meta=(AllowPrivateAccess=true))
	ECharacterClass CharacterClass=ECharacterClass::Warrior;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	TObjectPtr<UWidgetComponent> HealthBar;
};
