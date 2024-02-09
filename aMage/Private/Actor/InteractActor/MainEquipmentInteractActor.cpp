// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InteractActor/MainEquipmentInteractActor.h"

AMainEquipmentInteractActor::AMainEquipmentInteractActor()
{
	WeaponEquipMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponEquipMesh");
	WeaponEquipMesh->SetupAttachment(CustomRootComponent);
}
