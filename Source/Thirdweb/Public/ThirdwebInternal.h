// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Containers/UnrealString.h"

#include "Templates/SharedPointer.h"

// Created as a struct instead of a namespaces so the functions are not exported.
struct FThirdwebAnalytics
{

	static void SendConnectEvent(const FString& Wallet, const FString& Type);

	static FString GenerateUUID();
private:
	static FString JsonObjectToString(const TSharedPtr<class FJsonObject>& JsonObject);
	static FString GetPluginVersion();
};

