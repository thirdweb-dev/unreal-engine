// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#include "ThirdwebEditorStyle.h"

#include "ThirdwebEditorModule.h"
#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IPluginManager.h"
#include "Slate/SlateGameResources.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateStyleRegistry.h"

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FThirdwebEditorStyle::StyleInstance = nullptr;

void FThirdwebEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FThirdwebEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FThirdwebEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("ThirdwebEditorStyle"));
	return StyleSetName;
}

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef<FSlateStyleSet> FThirdwebEditorStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("ThirdwebEditorStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("Thirdweb")->GetBaseDir() / TEXT("Resources"));

	Style->Set("LevelEditor.Thirdweb.Logo", new IMAGE_BRUSH_SVG(TEXT("ThirdwebIcon"), Icon20x20));
	Style->Set("LevelEditor.Thirdweb.Screenshot", new IMAGE_BRUSH_SVG(TEXT("ScreenshotIcon"), Icon20x20));
	return Style;
}

void FThirdwebEditorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FThirdwebEditorStyle::Get()
{
	return *StyleInstance;
}
