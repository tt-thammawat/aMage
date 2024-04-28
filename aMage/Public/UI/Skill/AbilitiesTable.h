// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/MainPaintWidget.h"
#include "AbilitiesTable.generated.h"


USTRUCT(BlueprintType)
struct FAbilitiesTagListGroup
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag GroupGameplayTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAbilitiesTagList> AbilitiesTagLists;
};

/**
 * 
 */
UCLASS()
class AMAGE_API UAbilitiesTable : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void InitializeAbilitiesToTMap(const TArray<FAbilitiesTagList>& AbilitiesTagLists);
	UFUNCTION(BlueprintImplementableEvent)
	void TableTagMapSet();
protected:
	//TODO: Change This TO TMap or Something
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<FAbilitiesTagListGroup> AllGroups;
};
