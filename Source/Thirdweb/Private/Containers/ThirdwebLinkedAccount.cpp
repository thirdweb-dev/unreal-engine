﻿// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Containers/ThirdwebLinkedAccount.h"

FThirdwebLinkedAccount FThirdwebLinkedAccount::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebLinkedAccount LinkedAccount;
	if (JsonObject.IsValid())
	{
		if (JsonObject->HasTypedField<EJson::String>(TEXT("type")))
		{
			LinkedAccount.Type = JsonObject->GetStringField(TEXT("type"));
		}
		if (JsonObject->HasTypedField<EJson::Object>(TEXT("details")))
		{
			const TSharedPtr<FJsonObject> DetailsJsonObject = JsonObject->GetObjectField(TEXT("details"));
			if (JsonObject->HasTypedField<EJson::String>(TEXT("id")))
			{
				LinkedAccount.Id = FName(*JsonObject->GetStringField(TEXT("id")));
			}
			if (!JsonObject->HasTypedField<EJson::Null>(TEXT("email")) && JsonObject->HasTypedField<EJson::String>(TEXT("email")))
			{
				LinkedAccount.Email = JsonObject->GetStringField(TEXT("email"));
			}
			if (!JsonObject->HasTypedField<EJson::Null>(TEXT("address")) && JsonObject->HasTypedField<EJson::String>(TEXT("address")))
			{
				LinkedAccount.Address = JsonObject->GetStringField(TEXT("address"));
			}
			if (!JsonObject->HasTypedField<EJson::Null>(TEXT("phone")) && JsonObject->HasTypedField<EJson::String>(TEXT("phone")))
			{
				LinkedAccount.Phone = JsonObject->GetStringField(TEXT("phone"));
			}
		}
	}
	return LinkedAccount;
}

