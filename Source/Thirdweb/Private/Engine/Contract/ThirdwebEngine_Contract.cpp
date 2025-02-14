// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#include "Engine/Contract/ThirdwebEngine_Contract.h"

#include "ThirdwebLog.h"
#include "ThirdwebUtils.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Transaction/ThirdwebEngineTransactionOverrides.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Internal/ThirdwebHeaders.h"
#include "Internal/ThirdwebURLSearchParams.h"
#include "Kismet/KismetStringLibrary.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"

namespace ThirdwebEngine::Contract
{
	FString FormatUrl(const int64 ChainId, const FString& ContractAddress, const FString& Endpoint, const FThirdwebURLSearchParams& Params)
	{
		return ThirdwebEngine::FormatUrl(TEXT("contract"), FString::Printf(TEXT("%lld/%s/%s"), ChainId, *ContractAddress, *Endpoint), Params);
	}

	void Read(
		const UObject* Outer,
		const int64 ChainId,
		const FString& ContractAddress,
		const FString& FunctionName,
		const TArray<FString>& Args,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("functionName"), FunctionName);
		Params.Set(TEXT("args"), UKismetStringLibrary::JoinStringArray(Args, TEXT(",")), Args.Num() > 0);
		Request->SetURL(FormatUrl(ChainId, ContractAddress, TEXT("read"), Params));
		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Contract::Read::Content=%s"), *Content)
			FString Error;
			if (TSharedPtr<FJsonValue> JsonValue; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonValue, Error))
			{
				EXECUTE_IF_BOUND(SuccessDelegate, ThirdwebUtils::Json::ToString(JsonValue))
			}
			else
			{
				EXECUTE_IF_BOUND(ErrorDelegate, Error)
			}
		});
		Request->ProcessRequest();
	}

	void Write(
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
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest(TEXT("POST"));
		FThirdwebHeaders Headers;
		Headers.Set(TEXT("x-backend-wallet-address"), BackendWalletAddress);
		Headers.Set(TEXT("x-idempotency-key"), IdempotencyKey);
		Headers.Set(TEXT("x-account-address"), SmartWallet.ToAddress(), SmartWallet.IsValid());
		if (FString Factory = SmartWallet.GetCustomFactory(); !Factory.IsEmpty())
		{
			Headers.Set(TEXT("x-account-factory-address"), Factory);
		}
		
		Headers.UpdateRequest(Request);
		
		if (!Data.IsValid())
		{
			ErrorDelegate.Execute(TEXT("Data object is required"));
			return;
		}
		if (!Data->HasTypedField<EJson::String>(TEXT("functionName")) || Data->GetStringField(TEXT("functionName")).IsEmpty())
		{
			ErrorDelegate.Execute(TEXT("Data object must contain a functionName field"));
			return;
		}
		if (!Data->HasField(TEXT("args")))
		{
			ErrorDelegate.Execute(TEXT("Data object must contain an args field"));
			return;
		}
		Request->SetContentAsString(ThirdwebUtils::Json::ToString(Data));

		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("simulateTx"), true, bSimulateTx);
		Request->SetURL(FormatUrl(ChainId, ContractAddress, TEXT("write"), Params));
		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Contract::Write::Content=%s"), *Content)
			FString Error;
			if (TSharedPtr<FJsonObject> JsonObject; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonObject, Error))
			{
				FString QueueId = TEXT("Unknown");
				if (JsonObject->HasTypedField<EJson::String>(TEXT("queueId")))
				{
					QueueId = JsonObject->GetStringField(TEXT("queueId"));
				}
				EXECUTE_IF_BOUND(SuccessDelegate, QueueId)
			}
			else
			{
				EXECUTE_IF_BOUND(ErrorDelegate, Error)
			}
		});
		Request->ProcessRequest();
	}
}
