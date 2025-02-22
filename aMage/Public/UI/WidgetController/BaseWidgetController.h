// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	// Define The Constructor
	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC,APlayerState* PS,UAbilitySystemComponent* ASC,UAttributeSet* AS)
	: PlayerController(PC),PlayerState(PS),AbilitySystemComponent(ASC),AttributeSet(AS) {}
	// End Constructor
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class AMAGE_API UBaseWidgetController : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParam(const FWidgetControllerParams& WCParams);

	virtual void BroadcastInitialValue();
	virtual void BindCallbacksToDependencies();
protected:
	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;
	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
	
private:
};
