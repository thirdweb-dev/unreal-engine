// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#pragma once

struct FThirdwebURLSearchParams;

#include "Engine/BackendWallet/ThirdwebEngine_BackendWallet.h"
#include "Engine/Contract/ThirdwebEngine_Contract.h"
#include "Engine/Marketplace/ThirdwebEngine_Marketplace.h"
#include "Engine/Transaction/ThirdwebEngine_Transaction.h"

namespace ThirdwebEngine
{
	FString FormatUrl(const FString& Base, const FString& Endpoint, const FThirdwebURLSearchParams& Params);
}
