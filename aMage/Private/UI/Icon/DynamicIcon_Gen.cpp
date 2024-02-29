// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Icon/DynamicIcon_Gen.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

bool UDynamicIcon_Gen::CaptureMeshWithCameraAndBounds(USceneCaptureComponent2D* SceneCaptureComponent,
                                                      float SphereRadius, float BoundingBoxExtentY, float BoundingBoxExtentZ)
{
	if (!SceneCaptureComponent)
	{
		return false;
	}
	
	float NewCameraX = BoundingBoxExtentY + BoundingBoxExtentZ+SphereRadius;

	FVector CurrentLocation = SceneCaptureComponent->GetRelativeLocation();

	FVector NewCameraLocation = FVector(NewCameraX, CurrentLocation.Y, CurrentLocation.Z);

	SceneCaptureComponent->SetRelativeLocation(-NewCameraLocation);

	// Capture the scene.
	SceneCaptureComponent->CaptureScene();

	return true;
}

void UDynamicIcon_Gen::CreateIcon(UTextureRenderTarget2D* TextureRenderTarget, UTexture2D*& OutputIcon)
{
	if (!TextureRenderTarget)
	{
		OutputIcon = nullptr;
		return;
	}

	// Create a new Texture2D to store the RenderTarget content.
	UTexture2D* Texture = UTexture2D::CreateTransient(TextureRenderTarget->SizeX, TextureRenderTarget->SizeY, PF_B8G8R8A8);

#if WITH_EDITORONLY_DATA
	Texture->MipGenSettings = TMGS_NoMipmaps;
#endif

	// Read the pixels from the RenderTarget and store them in a FColor array.
	TArray<FColor> SurfData;
	FRenderTarget* RenderTargetResource = TextureRenderTarget->GameThread_GetRenderTargetResource();

	if (RenderTargetResource)
	{
		RenderTargetResource->ReadPixels(SurfData);
	}

	// Apply alpha threshold operation: Set the alpha to 0 for black pixels, leave it unchanged for others.
	for (FColor& Pixel : SurfData)
	{
		if (Pixel.R == 0 && Pixel.G == 0 && Pixel.B == 0) // Check if the pixel is black
		{
			Pixel.A = 0; // Set alpha to 0 for black pixels
		}
	}
	// Lock and copy the data between the textures.
	void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	const int32 TextureDataSize = SurfData.Num() * sizeof(FColor);
	FMemory::Memcpy(TextureData, SurfData.GetData(), TextureDataSize);
	Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

	// Apply Texture changes to GPU memory.
	Texture->UpdateResource();

	// Set the output icon.
	OutputIcon = Texture;
}

bool UDynamicIcon_Gen::SaveTextureToPNG(UTexture2D* Texture, const FString& FilePath, const FString& FileName,
	FString& OutputFilePath)
{
    if (!Texture || FilePath.IsEmpty() || FileName.IsEmpty())
    {
        // Handle invalid inputs
        UE_LOG(LogTemp, Error, TEXT("invalid inputs"));
        return false;
    }

    // Read the pixels from the Texture2D and store them in a FColor array.
    TArray<FColor> SurfData;
    void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_ONLY);
    const int32 TextureDataSize = Texture->GetPlatformData()->Mips[0].BulkData.GetBulkDataSize();
    SurfData.SetNumUninitialized(TextureDataSize / sizeof(FColor));
    FMemory::Memcpy(SurfData.GetData(), TextureData, TextureDataSize);
    Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

    // Create an image wrapper (PNG format)
    IImageWrapperModule* ImageWrapperModule = FModuleManager::LoadModulePtr<IImageWrapperModule>(FName("ImageWrapper"));

    if (!ImageWrapperModule)
    {
        // Image wrapper module not available
        UE_LOG(LogTemp, Error, TEXT("Image wrapper module not available"));
        return false; 
    }

    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule->CreateImageWrapper(EImageFormat::PNG);

    if (!ImageWrapper.IsValid())
    {
        // Image wrapper creation failed
        UE_LOG(LogTemp, Error, TEXT("Image wrapper creation failed"));
        return false; 
    }

    // Set the raw data and image size
    ImageWrapper->SetRaw(SurfData.GetData(), SurfData.Num() * sizeof(FColor), Texture->GetSizeX(), Texture->GetSizeY(), ERGBFormat::BGRA, 8);

    // Encode the image data
    const TArray<uint8, FDefaultAllocator64>& EncodedData = ImageWrapper->GetCompressed(0);

    // Set the filename for the image
    const FString FullFilePath = FPaths::Combine(FilePath, FileName);

    // Save the encoded data to a file
    if (!FFileHelper::SaveArrayToFile(EncodedData, *FullFilePath))
    {
        // Failed to save image
        UE_LOG(LogTemp, Error, TEXT("Failed to save PNG to %s"), *FullFilePath);
        return false;
    }
	
    // Assign the output filepath
    OutputFilePath = FullFilePath;
    // Replace forward slashes with backslashes
    OutputFilePath.ReplaceInline(TEXT("/"), TEXT("\\"));

    return true;

}
