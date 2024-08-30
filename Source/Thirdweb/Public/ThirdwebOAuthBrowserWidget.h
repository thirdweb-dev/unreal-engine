﻿// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#if WITH_CEF
#include "SWebBrowser.h"
#endif
#include "Components/Widget.h"
#include "ThirdwebWalletHandle.h"
#include "ThirdwebOAuthBrowserWidget.generated.h"

UCLASS()
class THIRDWEB_API UThirdwebOAuthBrowserWidget : public UWidget
{
	GENERATED_BODY()

public:
	//~ Begin Public Overrides
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
	//~ End Public Overrides

	virtual void HandleUrlChanged(const FText& InUrl);
	bool IsPageLoaded() const;
	
	UFUNCTION(BlueprintCallable, Category="Thirdweb|OAuth Browser")
	void Authenticate(const FString& OAuthLoginUrl);
	
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void OnWidgetRebuilt() override;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUrlChanged,const FString&, InUrl);
	FOnUrlChanged OnUrlChanged;
	
	static const FString DummyUrl;
	
private:
	FString InitialUrl;
	bool bSupportsTransparency = false;
	bool bShowInitialThrobber = false;
	
#if WITH_CEF
	TSharedPtr<SWebBrowser> Browser;
#endif
};
