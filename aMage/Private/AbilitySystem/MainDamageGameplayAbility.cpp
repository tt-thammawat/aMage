// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MainDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UMainDamageGameplayAbility::CauseDamages(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass,1);
	for(TTuple<FGameplayTag, FScalableFloat>& Pair : DamageTypes)
	{
		if(!Pair.Value.Curve.IsNull())
		{
			float DamageMagnitude = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,Pair.Key,DamageMagnitude);
		}
		
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FTaggedMontage UMainDamageGameplayAbility::GetRandomTaggedMontageFromArray(
	const TArray<FTaggedMontage>& TaggedMontages) const
{
	if(TaggedMontages.Num()>0)
	{
		const int32 Selection = FMath::RandRange(0,TaggedMontages.Num() - 1);
		return TaggedMontages[Selection];
	}
	return FTaggedMontage();
}
