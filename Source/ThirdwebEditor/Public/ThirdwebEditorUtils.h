// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#pragma once

#include "ISettingsModule.h"
#include "Modules/ModuleManager.h"

namespace ThirdwebEditorUtils
{
	template <typename T = UDeveloperSettings>
	void OpenDeveloperSettings()
	{
		const T* Settings = GetDefault<T>();
		FModuleManager::LoadModuleChecked<ISettingsModule>("Settings").ShowViewer(
			Settings->GetContainerName(), 
			Settings->GetCategoryName(), 
			Settings->GetSectionName()
		);
	}
}
