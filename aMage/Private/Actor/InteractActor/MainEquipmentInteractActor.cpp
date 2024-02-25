// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InteractActor/MainEquipmentInteractActor.h"

AMainEquipmentInteractActor::AMainEquipmentInteractActor()
{
	bReplicates = true;

	WeaponEquipMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponEquipMesh");
	WeaponEquipMesh->SetupAttachment(CustomRootComponent);
}
