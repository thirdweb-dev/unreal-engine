// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#pragma once

#include "Styling/SlateStyle.h"

class FThirdwebEditorStyle
{
public:
	static void Initialize();

	static void Shutdown();
	
	static void ReloadTextures();
	
	static const ISlateStyle& Get();

	static FName GetStyleSetName();

private:
	static TSharedRef<class FSlateStyleSet> Create();

private:
	static TSharedPtr<class FSlateStyleSet> StyleInstance;
};
