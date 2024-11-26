﻿// Copyright (c) 2024 Thirdweb. All Rights Reserved.

// ReSharper disable CppUE4CodingStandardNamingViolationWarning,CppUE4CodingStandardNamingViolationWarning
#pragma once

#include "Delegates/Delegate.h" 
#include "Delegates/DelegateCombinations.h"

#define TO_RUST_STRING(v) v.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*v)

#define TO_RUST_TIMESTAMP(v) v == FDateTime::MinValue() ? 0 : v.ToUnixTimestamp()

#define EXECUTE_IF_BOUND(Delegate, ...) \
	if (Delegate.IsBound()) { \
		Delegate.Execute(__VA_ARGS__); \
	}

#define CHECK_NETWORK \
	if (!bConnectedSuccessfully) { \
		EXECUTE_IF_BOUND(ErrorDelegate, TEXT("Network Connection Failed")) \
		return; \
	}

#define HTTP_LAMBDA_PARAMS FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully

#define HANDLE_QUEUE_ID_RESPONSE \
	FString Error; \
	if (TSharedPtr<FJsonObject> QueueJsonObject; ThirdwebUtils::Json::ParseEngineResponse(Content, QueueJsonObject, Error)) \
	{ \
		if (QueueJsonObject.IsValid() && QueueJsonObject->HasTypedField<EJson::String>(TEXT("queueId"))) \
		{ \
			EXECUTE_IF_BOUND(SuccessDelegate, QueueJsonObject->GetStringField(TEXT("queueId"))) \
		} \
	} \
	else \
	{ \
		EXECUTE_IF_BOUND(ErrorDelegate, Error) \
	}

DECLARE_DELEGATE_OneParam(FStringDelegate, const FString&);
DECLARE_DELEGATE_OneParam(FStringArrayDelegate, const TArray<FString>&);
DECLARE_DELEGATE_OneParam(FBoolDelegate, const bool);
DECLARE_DELEGATE_OneParam(FInt32Delegate, const int32);