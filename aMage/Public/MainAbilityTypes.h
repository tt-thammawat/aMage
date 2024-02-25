#pragma once


#include "GameplayEffectTypes.h"
#include "MainAbilityTypes.generated.h"


USTRUCT(BlueprintType)
struct FMainGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FMainGameplayEffectContext* Duplicate() const override
	{
		FMainGameplayEffectContext* NewContext = new FMainGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	//Optimize struct for serialize 
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
protected:
	
	UPROPERTY()
	bool bIsFireDamage = false;
	UPROPERTY()
	bool bIsLightningDamage = false;
	UPROPERTY()
	bool bIsIceDamage = false;
	UPROPERTY()
	bool bIsPhysicalDamage = false;
	
public:

	bool IsFireDamage() const {return bIsFireDamage;};
	void SetIsFireDamage(bool bInIsFireDamage) {bIsFireDamage = bInIsFireDamage;};
	bool IsLightningDamage() const {return bIsLightningDamage;};
	void SetIsLightningDamage(bool bInIsLightningDamage) {bIsLightningDamage = bInIsLightningDamage;};
	bool IsIceDamage() const {return bIsIceDamage;};
	void SetIsIceDamage(bool bInIsIceDamage) {bIsIceDamage = bInIsIceDamage;};
	bool IsPhysicalDamage() const {return bIsPhysicalDamage;};
	void SetIsPhysicalDamage(bool bInIsPhysicalDamage) {bIsPhysicalDamage = bInIsPhysicalDamage;};
};

template<>
struct TStructOpsTypeTraits<FMainGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FMainGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy=true
	};
};
