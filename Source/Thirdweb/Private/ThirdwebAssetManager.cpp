// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#include "ThirdwebAssetManager.h"

UThirdwebAssetManager::UThirdwebAssetManager()
{
	// Default materials removed, assuming new ones will be set up later or are no longer needed.
}

void UThirdwebAssetManager::LoadDefaultMaterials()
{
	// No materials to load now.
}

UMaterial* UThirdwebAssetManager::GetDefaultMaterial()
{
	return nullptr; // Return nullptr or a new default material if necessary.
}

UMaterial* UThirdwebAssetManager::GetDefaultTranslucentMaterial()
{
	return nullptr; // Return nullptr or a new default translucent material if necessary.
}
