﻿// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Browser/ThirdwebOAuthBrowserWidget.h"

#include "ThirdwebLog.h"

#include "Async/Async.h"

#include "GenericPlatform/GenericPlatformHttp.h"

#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#if WITH_CEF
#include "SWebBrowser.h"
#endif

#define ENSURE_VALID_BROWSER(FunctionName) \
	if (!Browser.IsValid()) \
	{ \
		TW_LOG(Error, TEXT("OAuthBrowserWidget::%s::Web browser invalid"), TEXT(FunctionName)); \
		return; \
	}

const FString UThirdwebOAuthBrowserWidget::DummyUrl = TEXT("about:blank");

bool UThirdwebOAuthBrowserWidget::IsPageLoaded() const
{
#if WITH_CEF
	return Browser.IsValid() && Browser->IsLoaded();
#else
	return false;
#endif
}

FString UThirdwebOAuthBrowserWidget::GetUrl() const
{
#if WITH_CEF
	if (Browser.IsValid())
	{
		if (FString Url = Browser->GetUrl(); !Url.IsEmpty())
		{
			return FGenericPlatformHttp::UrlDecode(Url);
		}
	}
#endif
	return TEXT("");
}

void UThirdwebOAuthBrowserWidget::ReleaseSlateResources(const bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
#if WITH_CEF
	Browser.Reset();
#endif
}

#if WITH_EDITOR
const FText UThirdwebOAuthBrowserWidget::GetPaletteCategory()
{
	return NSLOCTEXT("Thirdweb", "Thirdweb", "Thirdweb");
}
#endif

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
void UThirdwebOAuthBrowserWidget::Authenticate(const FString& OAuthLoginUrl)
{
#if WITH_CEF
	ENSURE_VALID_BROWSER("Authenticate")
	TW_LOG(Verbose, TEXT("OAuthBrowserWidget::Authenticate::Loading %s"), *OAuthLoginUrl);
	Browser->LoadURL(OAuthLoginUrl);
#endif
}

void UThirdwebOAuthBrowserWidget::HandleUrlChanged(const FText& InUrl)
{
#if WITH_CEF
	ENSURE_VALID_BROWSER("HandleUrlChanged")
	FString Url = InUrl.ToString();
	TW_LOG(Verbose, TEXT("UThirdwebOAuthBrowserWidget::HandleUrlChanged:%s"), *Url);
	if (Url.IsEmpty())
	{
		Url = Browser->GetUrl();
	}
	TW_LOG(Verbose, TEXT("UThirdwebOAuthBrowserWidget::HandleUrlChanged:%s"), *Url);
	// Ensure this code runs on the game thread
	AsyncTask(ENamedThreads::GameThread, [&, Url]() { if (IsInGameThread()) OnUrlChanged.Broadcast(FGenericPlatformHttp::UrlDecode(Url)); });
#endif
}

void UThirdwebOAuthBrowserWidget::HandleOnLoadComplete()
{
#if WITH_CEF
	ENSURE_VALID_BROWSER("HandleOnLoadComplete")
	FString Url = Browser->GetUrl();
	AsyncTask(ENamedThreads::GameThread, [&, Url]() { if (IsInGameThread()) OnPageLoaded.Broadcast(FGenericPlatformHttp::UrlDecode(Url)); });
#endif
}

TSharedRef<SWidget> UThirdwebOAuthBrowserWidget::RebuildWidget()
{
#if WITH_CEF
	if (!IsDesignTime())
	{
		Browser = SNew(SWebBrowser)
			.InitialURL(InitialUrl)
			.ShowControls(false)
			.SupportsTransparency(bSupportsTransparency)
			.ShowInitialThrobber(bShowInitialThrobber)
			.OnLoadCompleted(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleOnLoadComplete))
			.OnUrlChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleUrlChanged));

		return Browser.ToSharedRef();
	}
#endif
	return SNew(SBox).HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			SNew(STextBlock).Text(NSLOCTEXT("Thirdweb", "Thirdweb OAuth Browser", "Thirdweb OAuth Browser"))
		];
}

void UThirdwebOAuthBrowserWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
}
