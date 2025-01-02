// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/AsyncTaskThirdwebBase.h"

#include "Wallets/ThirdwebSmartWalletHandle.h"

#include "AsyncTaskThirdwebSmartBase.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebSmartBase : public UAsyncTaskThirdwebBase
{
	GENERATED_BODY()

protected:	
	UPROPERTY(Transient)
	FSmartWalletHandle SmartWallet;
};
