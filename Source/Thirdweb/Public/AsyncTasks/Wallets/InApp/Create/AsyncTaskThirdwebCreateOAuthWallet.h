// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebInAppCreateWalletBase.h"
#include "AsyncTaskThirdwebCreateOAuthWallet.generated.h"

enum class EThirdwebOAuthProvider : uint8;

UCLASS(meta=(HasDedicatedAsyncNode))
class THIRDWEB_API UAsyncTaskThirdwebCreateOAuthWallet : public UAsyncTaskThirdwebInAppCreateWalletBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Wallets|InApp")
	static UAsyncTaskThirdwebCreateOAuthWallet* CreateOAuthWallet(UObject* WorldContextObject, const EThirdwebOAuthProvider Provider)
	{
		NEW_TASK
		Task->Provider = Provider;
		RR_TASK
	}

protected:
	UPROPERTY(Transient)
	EThirdwebOAuthProvider Provider;

public:
	virtual void Activate() override;
};
