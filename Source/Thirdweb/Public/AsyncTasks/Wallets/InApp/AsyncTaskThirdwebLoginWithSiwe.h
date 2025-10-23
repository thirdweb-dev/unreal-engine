// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebInAppBase.h"
#include "Wallets/ThirdwebInAppWalletHandle.h"
#include "AsyncTaskThirdwebLoginWithSiwe.generated.h"

class UThirdwebOAuthBrowserUserWidget;

/**
 * High-level SIWE login async task that handles browser widget creation and
 * authentication flow
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebLoginWithSiwe : public UAsyncTaskThirdwebInAppBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebLoginWithSiwe*
	LoginWithSiwe(UObject* WorldContextObject, const FInAppWalletHandle& Wallet);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSiweCompleteDelegate, const FString &, Payload, const FString &, Signature);

	UPROPERTY(BlueprintAssignable)
	FSiweCompleteDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Failed;

protected:
	UPROPERTY(Transient)
	UThirdwebOAuthBrowserUserWidget* Browser;

	UPROPERTY(Transient)
	FInAppWalletHandle Wallet;

private:
	UFUNCTION()
	void HandleSiweComplete(const FString& Payload, const FString& Signature);

	UFUNCTION()
	void HandleFailed(const FString& Error);
};
