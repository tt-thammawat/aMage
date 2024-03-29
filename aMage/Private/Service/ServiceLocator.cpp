// Fill out your copyright notice in the Description page of Project Settings.


#include "Service/ServiceLocator.h"


void FServiceLocator::RegisterService(const FName ServiceName, AActor* Service)
{
    if (Service && !Services.Contains(ServiceName))
	{
		Services.Add(ServiceName, Service);
    	OnServiceRegistered.ExecuteIfBound(ServiceName);
	}
}

AActor* FServiceLocator::GetService(const FName ServiceName)
{
	AActor* FoundService = Services.Find(ServiceName)->Get();
	return FoundService ? FoundService : nullptr;
}

void FServiceLocator::DeregisterService(const FName ServiceName)
{
    if (Services.Contains(ServiceName))
	{
		Services.Remove(ServiceName);
	}
}
