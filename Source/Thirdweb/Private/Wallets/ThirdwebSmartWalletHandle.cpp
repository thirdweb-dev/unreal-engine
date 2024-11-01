﻿// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Wallets/ThirdwebSmartWalletHandle.h"

#include "Thirdweb.h"
#include "ThirdwebCommon.h"
#include "ThirdwebLog.h"
#include "ThirdwebMacros.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebUtils.h"

#include "Containers/ThirdwebSigner.h"

#include "Dom/JsonObject.h"

#include "GenericPlatform/GenericPlatformHttp.h"

#include "Kismet/KismetStringLibrary.h"

#include "Misc/DefaultValueHelper.h"

#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#include "Tasks/Task.h"

FSmartWalletHandle::FSmartWalletHandle(const FInAppWalletHandle& InInAppWallet, const FString& Int64String)
{
	Type = Smart;
	int64 InID;
	InAppWallet = InInAppWallet;
	FDefaultValueHelper::ParseInt64(Int64String, InID);
	ensureAlwaysMsgf(InID > 0, TEXT("Invalid id 0"));
	ID = InID;
}

void FSmartWalletHandle::Create(const FInAppWalletHandle& InInAppWallet,
                                const int64 ChainID,
                                const bool bGasless,
                                const FString& Factory,
                                const FString& AccountOverride,
                                const FCreateSmartWalletDelegate& SuccessDelegate,
                                const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate);
	
	if (!InInAppWallet.IsValid())
	{
		if (ErrorDelegate.IsBound())
		{
			ErrorDelegate.Execute(TEXT("Invalid InApp wallet"));
		}
		return;
	}
	
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [InInAppWallet, ChainID, bGasless, Factory, AccountOverride, SuccessDelegate, ErrorDelegate]
	{
		if (FString Error; Thirdweb::create_smart_wallet(
			TO_RUST_STRING(UThirdwebRuntimeSettings::GetClientId()),
			TO_RUST_STRING(UThirdwebRuntimeSettings::GetBundleId()),
			nullptr,
			InInAppWallet.GetID(),
			TO_RUST_STRING(FString::Printf(TEXT("%lld"), ChainID)),
			bGasless,
			TO_RUST_STRING(Factory),
			TO_RUST_STRING(AccountOverride)
		).AssignResult(Error))
		{
			const FSmartWalletHandle SmartWallet = FSmartWalletHandle(InInAppWallet, Error);
			SuccessDelegate.Execute(SmartWallet);
			ThirdwebUtils::Internal::SendConnectEvent(SmartWallet);
		}
		else
		{
			if (ErrorDelegate.IsBound())
			{
				ErrorDelegate.Execute(Error);
			}
		}
	});
}

void FSmartWalletHandle::IsDeployed(const FBoolDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate);
	CHECK_VALIDITY(ErrorDelegate);
	
	FSmartWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, SuccessDelegate, ErrorDelegate]
	{
		if (FString Error; Thirdweb::smart_wallet_is_deployed(ThisCopy.GetID()).AssignResult(Error))
		{
			SuccessDelegate.Execute(Error.ToBool());
		}
		else
		{
			ErrorDelegate.Execute(Error);
		}
	});
}

void FSmartWalletHandle::CreateSessionKey(const FString& Signer,
                                          const TArray<FString>& ApprovedTargets,
                                          const FString& NativeTokenLimitPerTransactionInWei,
                                          const FDateTime& PermissionEnd,
                                          const FStringDelegate& SuccessDelegate,
                                          const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate);
	CHECK_VALIDITY(ErrorDelegate);
	
	FDateTime TenYearsFromNow = FDateTime::UtcNow() + FTimespan::FromDays(10 * 365);
	FDateTime EndTime = TenYearsFromNow;
	TArray<const char*> ApprovedTargetsCArray;
	for (const FString& Target : ApprovedTargets)
	{
		ApprovedTargetsCArray.Add(TO_RUST_STRING(Target));
	}
	if (PermissionEnd.ToUnixTimestamp() > 0)
	{
		EndTime = PermissionEnd;
		if (EndTime + FTimespan::FromMinutes(10) < FDateTime::UtcNow())
		{
			ErrorDelegate.Execute(TEXT("Invalid EndTime"));
			return;
		}
	}
	FSmartWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(
		UE_SOURCE_LOCATION,
		[ThisCopy, Signer, ApprovedTargets, ApprovedTargetsCArray, NativeTokenLimitPerTransactionInWei, EndTime, TenYearsFromNow, SuccessDelegate,
			ErrorDelegate]
		{
			if (FString Error; Thirdweb::smart_wallet_create_session_key(
				ThisCopy.GetID(),
				TO_RUST_STRING(Signer),
				ApprovedTargets.IsEmpty() ? nullptr : ApprovedTargetsCArray.GetData(),
				ApprovedTargetsCArray.Num(),
				TO_RUST_STRING(NativeTokenLimitPerTransactionInWei),
				0,
				TO_RUST_TIMESTAMP(EndTime),
				0,
				TO_RUST_TIMESTAMP(TenYearsFromNow)
			).AssignResult(Error))
			{
				TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
				const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Error);
				if (FJsonSerializer::Deserialize(Reader, JsonObject); JsonObject.IsValid())
				{
					if (JsonObject->HasTypedField<EJson::String>(TEXT("transactionHash")))
					{
						SuccessDelegate.Execute(JsonObject->GetStringField(TEXT("transactionHash")));
					}
				}
			}
			else
			{
				if (ErrorDelegate.IsBound())
				{
					ErrorDelegate.Execute(Error);
				}
			}
		});
}

void FSmartWalletHandle::RevokeSessionKey(const FString& Signer, const FSimpleDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate);
	CHECK_VALIDITY(ErrorDelegate);
	
	FSmartWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, Signer, SuccessDelegate, ErrorDelegate]
	{
		if (FString Error; Thirdweb::smart_wallet_revoke_session_key(ThisCopy.GetID(), TO_RUST_STRING(Signer)).AssignResult(Error))
		{
			SuccessDelegate.Execute();
		}
		else
		{
			ErrorDelegate.Execute(Error);
		}
	});
}

void FSmartWalletHandle::GetAdmins(const FStringArrayDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate);
	CHECK_VALIDITY(ErrorDelegate);
	
	FSmartWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, SuccessDelegate, ErrorDelegate]
	{
		TArray<FString> Admins;
		if (FString Error; Thirdweb::smart_wallet_get_all_admins(ThisCopy.GetID()).AssignResult(Error))
		{
			TArray<TSharedPtr<FJsonValue>> JsonValueArray;
			const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Error);
			FJsonSerializer::Deserialize(Reader, JsonValueArray);
			for (int i = 0; i < JsonValueArray.Num(); i++)
			{
				if (JsonValueArray[i]->Type == EJson::String)
				{
					Admins.Emplace(JsonValueArray[i]->AsString());
				}
			}
			SuccessDelegate.Execute(Admins);
		}
		else
		{
			if (ErrorDelegate.IsBound())
			{
				ErrorDelegate.Execute(Error);
			}
		}
	});
}

void FSmartWalletHandle::AddAdmin(const FString& Signer, const FSimpleDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate);
	CHECK_VALIDITY(ErrorDelegate);
	
	FSmartWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, Signer, SuccessDelegate, ErrorDelegate]
	{
		if (FString Error; Thirdweb::smart_wallet_add_admin(ThisCopy.GetID(), TO_RUST_STRING(Signer)).AssignResult(Error))
		{
			SuccessDelegate.Execute();
		} else
		{
			ErrorDelegate.Execute(Error);
		}
	});
}

void FSmartWalletHandle::RemoveAdmin(const FString& Signer, const FSimpleDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate);
	CHECK_VALIDITY(ErrorDelegate);
	
	FSmartWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, Signer, SuccessDelegate, ErrorDelegate]
	{
		if (FString Error; Thirdweb::smart_wallet_remove_admin(ThisCopy.GetID(), TO_RUST_STRING(Signer)).AssignResult(Error))
		{
			SuccessDelegate.Execute();
		} else
		{
			ErrorDelegate.Execute(Error);
		}
	});
}

void FSmartWalletHandle::GetActiveSigners(const FGetActiveSignersDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate);
	CHECK_VALIDITY(ErrorDelegate);
	
	FSmartWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, SuccessDelegate, ErrorDelegate]
	{
		if (FString Error; Thirdweb::smart_wallet_get_all_active_signers(ThisCopy.GetID()).AssignResult(Error))
		{
			TArray<FSigner> Signers;
			TArray<TSharedPtr<FJsonValue>> JsonValueArray;
			const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Error);
			FJsonSerializer::Deserialize(Reader, JsonValueArray);
			for (int i = 0; i < JsonValueArray.Num(); i++)
			{
				if (JsonValueArray[i]->Type == EJson::Object)
				{
					Signers.Emplace(FSigner::FromJson(JsonValueArray[i]->AsObject()));
				}
			}
			SuccessDelegate.Execute(Signers);
		}
		else
		{
			ErrorDelegate.Execute(Error);
		}
	});
}
