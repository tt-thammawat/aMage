#pragma once

UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	ETIP_Left UMETA(DiaplayName = "Turning Left"),
	ETIP_Right UMETA(DiaplayName = "Turning Right"),
	ETIP_NotTurning UMETA(DiaplayName = "Not Turning"),

	ETIP_Max UMETA(DiaplayName = "DefaultMAX")
};
