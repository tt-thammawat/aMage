// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DynamicIcon_Gen.generated.h"

/**
 * 
 */
UCLASS()
class AMAGE_API UDynamicIcon_Gen : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//Dynamic Icon Gen
	UFUNCTION(BlueprintCallable,Category = "MainAbilitySystemLibrary|DynamicIconGeneration")
	static bool CaptureMeshWithCameraAndBounds(USceneCaptureComponent2D* SceneCaptureComponent, 
		float SphereRadius, float BoundingBoxExtentY, float BoundingBoxExtentZ);
	
	UFUNCTION(BlueprintCallable, Category = "Icon Generator")
	static void CreateIcon(UTextureRenderTarget2D* TextureRenderTarget, UTexture2D*& OutputIcon);
	
	UFUNCTION(BlueprintCallable, Category = "Icon Generator")
	static bool SaveTextureToPNG(UTexture2D* Texture, const FString& FilePath, const FString& FileName, FString& OutputFilePath);
};
