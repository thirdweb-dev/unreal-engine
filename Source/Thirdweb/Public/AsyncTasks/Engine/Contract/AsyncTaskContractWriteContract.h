// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/AsyncTaskThirdwebBase.h"
#include "Engine/Transaction/ThirdwebEngineTransactionOverrides.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"
#include "AsyncTaskContractWriteContract.generated.h"

struct FJsonObjectWrapper;

UCLASS(meta=(HasDedicatedAsyncNode))
class THIRDWEB_API UAsyncTaskContractWriteContract : public UAsyncTaskThirdwebBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Engine|Contract")
	static UAsyncTaskContractWriteContract* WriteContract(
		UObject* WorldContextObject,
		const int64 ChainId,
		const FString& ContractAddress,
		const FString& BackendWalletAddress,
		const FSmartWalletHandle& SmartWallet,
		const FString& IdempotencyKey,
		const FString& FunctionName,
		const TArray<FString>& Args,
		const FThirdwebEngineTransactionOverrides& TxOverrides,
		const FString& Abi,
		const bool bSimulateTx
	);

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Engine|Contract")
	static UAsyncTaskContractWriteContract* WriteContractRaw(
		UObject* WorldContextObject,
		const int64 ChainId,
		const FString& ContractAddress,
		const FString& BackendWalletAddress,
		const FSmartWalletHandle& SmartWallet,
		const FString& IdempotencyKey,
		const FJsonObjectWrapper& Data,
		const bool bSimulateTx
	);

	virtual void Activate() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWriteContractDelegate, const FString&, QueueId, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FWriteContractDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FWriteContractDelegate Failed;

protected:
	UPROPERTY(Transient)
	int64 ChainId;

	UPROPERTY(Transient)
	FString ContractAddress;

	UPROPERTY(Transient)
	FString BackendWalletAddress;

	UPROPERTY(Transient)
	FSmartWalletHandle SmartWallet;

	UPROPERTY(Transient)
	FString IdempotencyKey;

	UPROPERTY(Transient)
	bool bSimulateTx;

	TSharedPtr<FJsonObject> Data;

private:
	virtual void HandleResponse(const FString& QueueId);
	void HandleFailed(const FString& Error);
};
