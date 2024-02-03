// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UI/WidgetController/BaseWidgetController.h"
#include "OverlayWidgetController.generated.h"

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText Message = FText();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<class UMainPlayerWidget> MessageWidget;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* Image = nullptr;
	
};

class UMainPlayerWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributesChangedSignature, float, NewValue);

//Delegate DataTableRowStruct
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature,FUIWidgetRow,Row);

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class AMAGE_API UOverlayWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnAttributesChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnAttributesChangedSignature OnMaxHealthChanged;
	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnAttributesChangedSignature OnManaChange;
	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnAttributesChangedSignature OnMaxmanaChange;
protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	//Template Function
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FMessageWidgetRowSignature MessageWidgetRow;
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	//will return all of the row if specifier match with the row
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}
