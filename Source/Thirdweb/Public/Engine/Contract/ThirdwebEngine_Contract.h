// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#pragma once

#include "Dom/JsonObject.h"
#include "ThirdwebMacros.h"

struct FThirdwebEngineTransactionOverrides;
struct FSmartWalletHandle;
struct FThirdwebURLSearchParams;

namespace ThirdwebEngine::Contract
{
	extern FString FormatUrl(const int64 ChainId, const FString& ContractAddress, const FString& Endpoint, const FThirdwebURLSearchParams& Params);

	extern THIRDWEB_API void Read(
		const UObject* Outer,
		const int64 ChainId,
		const FString& ContractAddress,
		const FString& FunctionName,
		const TArray<FString>& Args,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);
	
	extern THIRDWEB_API void Write(
		const UObject* Outer,
		const int64 ChainId,
		const FString& ContractAddress,
		const FString& BackendWalletAddress,
		const FSmartWalletHandle& SmartWallet,
		const FString& IdempotencyKey,
		const bool bSimulateTx,
		const TSharedPtr<FJsonObject>& Data,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);
}
