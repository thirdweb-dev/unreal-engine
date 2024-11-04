// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/AsyncTaskThirdwebBase.h"

#include "Engine/ThirdwebEngineTransactionOverrides.h"

#include "Interfaces/IHttpRequest.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"

#include "AsyncTaskThirdwebEngineWriteContract.generated.h"

UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebEngineWriteContract : public UAsyncTaskThirdwebBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="IdempotencyKey,FactoryAddress,TxOverrides,Abi,bSimulateTx", AutoCreateRefTerm="Args,SmartWallet,TxOverrides"), Category="Thirdweb|Engine")
	static UAsyncTaskThirdwebEngineWriteContract* WriteContract(UObject* WorldContextObject,
	                                                            const int64 ChainId,
	                                                            const FString& ContractAddress,
	                                                            const FString& BackendWalletAddress,
	                                                            const FSmartWalletHandle& SmartWallet,
	                                                            const FString& FactoryAddress,
	                                                            const FString& IdempotencyKey,
	                                                            const FString& FunctionName,
	                                                            const TArray<FString>& Args,
	                                                            const FThirdwebEngineTransactionOverrides& TxOverrides,
	                                                            const FString& Abi,
	                                                            const bool bSimulateTx);

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
	FString FactoryAddress;

	UPROPERTY(Transient)
	FString IdempotencyKey;

	UPROPERTY(Transient)
	FString FunctionName;

	UPROPERTY(Transient)
	TArray<FString> Args;

	UPROPERTY(Transient)
	FThirdwebEngineTransactionOverrides TxOverrides;

	UPROPERTY(Transient)
	FString Abi;

	UPROPERTY(Transient)
	bool bSimulateTx;

private:
	virtual void HandleResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void HandleFailed(const FString& Error);
};
