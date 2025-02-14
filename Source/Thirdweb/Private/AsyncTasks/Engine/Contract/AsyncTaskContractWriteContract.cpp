// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Contract/AsyncTaskContractWriteContract.h"

#include "JsonObjectWrapper.h"
#include "RHITransientResourceAllocator.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebUtils.h"
#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"

UAsyncTaskContractWriteContract* UAsyncTaskContractWriteContract::WriteContract(
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
)
{
	FJsonObjectWrapper JsonObjectWrapper = FJsonObjectWrapper();
	JsonObjectWrapper.JsonObject->SetStringField(TEXT("functionName"), FunctionName.TrimStartAndEnd());
	JsonObjectWrapper.JsonObject->SetArrayField(TEXT("args"), ThirdwebUtils::Json::ToJsonArray(Args));
	if (!TxOverrides.IsDefault())
	{
		JsonObjectWrapper.JsonObject->SetObjectField(TEXT("txOverrides"), TxOverrides.ToJson());
	}
	if (!Abi.IsEmpty())
	{
		JsonObjectWrapper.JsonObject->SetArrayField(TEXT("abi"), ThirdwebUtils::Json::ToJsonArray(Abi));
	}
	return WriteContractRaw(WorldContextObject, ChainId, ContractAddress, BackendWalletAddress, SmartWallet, IdempotencyKey, JsonObjectWrapper, bSimulateTx);
}

UAsyncTaskContractWriteContract* UAsyncTaskContractWriteContract::WriteContractRaw(
	UObject* WorldContextObject,
	const int64 ChainId,
	const FString& ContractAddress,
	const FString& BackendWalletAddress,
	const FSmartWalletHandle& SmartWallet,
	const FString& IdempotencyKey,
	const FJsonObjectWrapper& Data,
	const bool bSimulateTx
)
{
	NEW_TASK
	Task->ChainId = ChainId;
	Task->ContractAddress = ContractAddress.TrimStartAndEnd();
	Task->BackendWalletAddress = BackendWalletAddress.TrimStartAndEnd();
	Task->SmartWallet = SmartWallet;
	Task->IdempotencyKey = IdempotencyKey.TrimStartAndEnd();
	Task->bSimulateTx = bSimulateTx;
	Task->Data = Data.JsonObject;
	RR_TASK
}

void UAsyncTaskContractWriteContract::Activate()
{
	ThirdwebEngine::Contract::Write(
		this,
		ChainId,
		ContractAddress,
		BackendWalletAddress,
		SmartWallet,
		IdempotencyKey,
		bSimulateTx,
		Data,
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}

void UAsyncTaskContractWriteContract::HandleResponse(const FString& QueueId)
{
	Success.Broadcast(QueueId, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskContractWriteContract::HandleFailed(const FString& Error)
{
	Failed.Broadcast(TEXT(""), Error);
	SetReadyToDestroy();
}
