// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/AsyncTaskThirdwebBase.h"
#include "AsyncTaskContractReadContract.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskContractReadContract : public UAsyncTaskThirdwebBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AutoCreateRefTerm="Args"), Category="Thirdweb|Engine|Contract")
	static UAsyncTaskContractReadContract* ReadContract(UObject* WorldContextObject, const int64 ChainID, const FString& ContractAddress, const FString& FunctionName, const TArray<FString>& Args);

	virtual void Activate() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReadContractDelegate, const FString&, Result, const FString&, Error);
	UPROPERTY(BlueprintAssignable)
	FReadContractDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FReadContractDelegate Failed;

protected:
	UPROPERTY(Transient)
	int64 ChainId;
	
	UPROPERTY(Transient)
	FString ContractAddress;
	
	UPROPERTY(Transient)
	FString FunctionName;
	
	UPROPERTY(Transient)
	TArray<FString> Args;

private:
	virtual void HandleResponse(const FString& Content);
	void HandleFailed(const FString& Error);
};
