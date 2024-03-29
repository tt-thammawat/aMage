// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

DECLARE_DELEGATE_OneParam(FOnServiceRegistered, FName);

/**
 * 
 */
class AMAGE_API FServiceLocator
{
	
public:
	
	static FServiceLocator& Get()
	{
		static FServiceLocator Instance;
		return Instance;
	}

	FServiceLocator(const FServiceLocator&) = delete;
	FServiceLocator& operator=(const FServiceLocator&) = delete;
	
	FOnServiceRegistered OnServiceRegistered;

	void RegisterService(const FName ServiceName, AActor* Service);
	void DeregisterService(const FName ServiceName);
	AActor* GetService(const FName ServiceName);

	
private:
	FServiceLocator() {} // Private constructor
	TMap<FName, TObjectPtr<AActor>> Services;



};
