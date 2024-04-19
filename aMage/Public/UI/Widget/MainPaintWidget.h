// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/Widget/MainPlayerWidget.h"
#include "MainPaintWidget.generated.h"


class UGameplayAbility;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FAbilitiesTagList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameplayTag> Tags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> AbilityClass;
};

USTRUCT(BlueprintType)
struct FHintTagMatch
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SpellMatchName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Tag= FGameplayTag();
};

class AMainPlayerController;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDrawingRuneSuccessSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDrawingClearSpellSuccessSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDrawingReloadSpellSuccessSignature);

UCLASS()
class AMAGE_API UMainPaintWidget : public UMainPlayerWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable,Category="PlayerController")
	void SetUpMainPlayerController(APlayerController* PlayerController);
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UPROPERTY(BlueprintAssignable)
	FOnDrawingRuneSuccessSignature OnDrawingRuneSuccess;
	UPROPERTY(BlueprintAssignable)
	FOnDrawingClearSpellSuccessSignature OnClearSpellSuccess;
	UPROPERTY(BlueprintAssignable)
	FOnDrawingReloadSpellSuccessSignature OnReloadSpellSuccess;
	
	UFUNCTION(BlueprintCallable)
	TArray<FGameplayTag> GetRuneTags() const {return RuneTags;};
	
	UFUNCTION(BlueprintCallable)
	void SetIsStartFocus(bool bStartFocus) { bIsStartFocus = bStartFocus;};

	//This Case Just For The Granted Spell Abilities
	//Check If Spell Is Active and Set Rune Tag Accordingly
	virtual void CheckCurrentlyActiveSpell(TSubclassOf<UGameplayAbility>& AbilityClass);
	
	bool AreRuneTagsEqualInOrder(const TArray<FGameplayTag>& MappingTags);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ResetTagUI();
	
	//END This Case Just For The Granted Spell Abilities

protected:
	
	//If Match Add Sign If Not Remove Last RuneTag Index
	UFUNCTION(BlueprintCallable)
	bool CheckIfRuneTagMatchInList();

	//Clear In Blueprint
	UFUNCTION(BlueprintCallable)
	void K2_CallClearSpellFunction();

	
	UFUNCTION(BlueprintCallable)
	TArray<FHintTagMatch> SetHintTagMatchMapping();
	
private:
	
	void CheckDrawSpell();
	bool bIsStartFocus=false;
	bool bIsDrawing=false;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Player,meta=(AllowPrivateAccess=true))
	TObjectPtr<AMainPlayerController> MainPlayerController;

	//Initial Tag Lists
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Rune,meta=(AllowPrivateAccess=true))
	TArray<FAbilitiesTagList> RuneAbilitiesTagMatchesLists;

	//Save Tag From Drawing
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Rune,meta=(AllowPrivateAccess=true))
	TArray<FGameplayTag> RuneTags;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Sound,meta=(AllowPrivateAccess=true))
	TObjectPtr<USoundBase> SuccessSound;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Sound,meta=(AllowPrivateAccess=true))
	TObjectPtr<USoundBase> ReloadSound;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Sound,meta=(AllowPrivateAccess=true))
	TObjectPtr<USoundBase> FailSound;
	
//Drawing Canvas Line
	 
protected:
	virtual int32 NativePaint(const FPaintArgs &Args, const FGeometry &AllottedGeometry, const FSlateRect &MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle &InWidgetStyle, bool bParentEnabled) const override;

public:
	
	void AddPoint(const FVector2D &Point);
	UFUNCTION(BlueprintCallable,Category=Default)	
	void RemoveAllPoints();

private:
	//Line Visual
	TArray<TArray<FVector2D>> LineSegments;
	TArray<FVector2D> Points;
};
