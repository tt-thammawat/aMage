#pragma once

#include "EEffectPolicy.generated.h"


UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};


UENUM(BlueprintType)
enum class EEffectRemovePolicy
{
	RemoveOnOverlap,
	RemoveOnEndOverlap,
	DoNotReMove
};