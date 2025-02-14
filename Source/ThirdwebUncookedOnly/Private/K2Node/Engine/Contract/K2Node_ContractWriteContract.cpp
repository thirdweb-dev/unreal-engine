// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#include "K2Node/Engine/Contract/K2Node_ContractWriteContract.h"

#include "JsonObjectWrapper.h"
#include "AsyncTasks/Engine/Contract/AsyncTaskContractWriteContract.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"

namespace TwPins
{
	const FName ChainId = FName(TEXT("ChainId"));
	const FName ContractAddress = FName(TEXT("ContractAddress"));
	const FName BackendWalletAddress = FName(TEXT("BackendWalletAddress"));
	const FName IdempotencyKey = FName(TEXT("IdempotencyKey"));
	const FName FunctionName = FName(TEXT("FunctionName"));
	const FName Args = FName(TEXT("Args"));
	const FName TxOverrides = FName(TEXT("TxOverrides"));
	const FName Abi = FName(TEXT("Abi"));
	const FName SimulateTx = FName(TEXT("bSimulateTx"));
	const FName QueueId = FName(TEXT("QueueId"));
}

#define LOCTEXT_NAMESPACE "ThirdwebUncookedOnly"

UK2Node_ContractWriteContract::UK2Node_ContractWriteContract()
{
	ProxyClass = UAsyncTaskContractWriteContract::StaticClass();
	ProxyFactoryClass = UAsyncTaskContractWriteContract::StaticClass();
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskContractWriteContract, WriteContract);

	bRaw = false;
}

#if WITH_EDITOR
void UK2Node_ContractWriteContract::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UK2Node_ContractWriteContract, bRaw))
	{
		if (UpdatePins())
		{
			ReconstructNode();
		}
	}
}
#endif

FText UK2Node_ContractWriteContract::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("K2Node_ContractWriteContract_SmartNodeTitle", "Write Contract");
}

FText UK2Node_ContractWriteContract::GetTooltipText() const
{
	return LOCTEXT("K2Node_ContractWriteContract_TooltipText", "Write to a contract via an Engine instance");
}

FText UK2Node_ContractWriteContract::GetMenuCategory() const
{
	return LOCTEXT("K2Node_ContractWriteContract_Category", "Thirdweb|Engine|Contract");
}

bool UK2Node_ContractWriteContract::IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const
{
	if (MyPin == GetWalletPin() && OtherPin)
	{
		if (OtherPin->PinType.PinSubCategoryObject != TBaseStructure<FInAppWalletHandle>::Get() && OtherPin->PinType.PinSubCategoryObject != TBaseStructure<FSmartWalletHandle>::Get())
		{
			OutReason = LOCTEXT("K2Node_ContractWriteContract_IsConnectionDisallowed", "Only In App & Smart Wallet Handles allowed").ToString();
			return true;
		}
	}
	return Super::IsConnectionDisallowed(MyPin, OtherPin, OutReason);
}

void UK2Node_ContractWriteContract::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	Super::Super::PinDefaultValueChanged(Pin);
	UpdatePins();
}

void UK2Node_ContractWriteContract::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();
	SetNodeHasAdvanced(true);
	// Input Pins
	SetPinFriendlyName(
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Int64, TwPins::ChainId),
		LOCTEXT("K2Node_ContractWriteContract_ChainId", "Chain ID")
	);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::ContractAddress);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::BackendWalletAddress);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FSmartWalletHandle::StaticStruct(), TwPins::SmartWallet);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::FunctionName);
	FCreatePinParams ArgsPinParams = FCreatePinParams();
	ArgsPinParams.ContainerType = EPinContainerType::Array;
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::Args, ArgsPinParams);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, TwPins::SimulateTx);
	SetPinAdvancedView(CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FThirdwebEngineTransactionOverrides::StaticStruct(), TwPins::TxOverrides));
	SetPinAdvancedView(CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::Abi), true);
	SetPinVisibility(CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FJsonObjectWrapper::StaticStruct(), TwPins::Data), false)->PinToolTip =
		TEXT(
			"Raw payload for the write contract endpoint. Must match all fields defined in the api documentation\nExample Payload:\n{\n  \"functionName\": \"deposit\"\n  \"args\": [ 1000000 ]\n}\n\nDocumentation: https://thirdweb-engine.apidocumentation.com/reference#tag/contract/POST/contract/{chain}/{contractAddress}/write\n\nJson Object Wrapper\nJson Object Structure");
	SetPinAdvancedView(CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TwPins::IdempotencyKey), true);

	// Output Pins
	SetPinFriendlyName(
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_String, TwPins::QueueId),
		LOCTEXT("K2Node_ContractWriteContract_QueueId", "Queue ID")
	);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_String, TwPins::Error);

	UpdatePins();
}

void UK2Node_ContractWriteContract::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	if (const UEdGraphPin* Pin = GetSmartWalletPin())
	{
		if (bRaw)
		{
			ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskContractWriteContract, WriteContractRaw);
		}
		else
		{
			ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskContractWriteContract, WriteContract);
		}
	}
	RemoveHiddenPins(this);
	Super::ExpandNode(CompilerContext, SourceGraph);
}

bool UK2Node_ContractWriteContract::UpdatePins()
{
	SetPinVisibility(GetFunctionNamePin(), !bRaw);
	SetPinVisibility(GetArgsPin(), !bRaw);
	SetPinVisibility(GetTxOverridesPin(), !bRaw);
	SetPinVisibility(GetAbiPin(), !bRaw);
	SetPinVisibility(GetDataPin(), bRaw);
	if (UEdGraph* Graph = GetGraph())
	{
		Graph->NotifyGraphChanged();
	}
	return true;
}

UEdGraphPin* UK2Node_ContractWriteContract::GetChainIdPin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::ChainId);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ContractWriteContract::GetContractAddressPin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::ContractAddress);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ContractWriteContract::GetBackendWalletAddressPin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::BackendWalletAddress);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ContractWriteContract::GetIdempotencyKeyPin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::IdempotencyKey);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ContractWriteContract::GetFunctionNamePin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::FunctionName);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ContractWriteContract::GetArgsPin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::Args);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ContractWriteContract::GetTxOverridesPin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::TxOverrides);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ContractWriteContract::GetAbiPin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::Abi);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ContractWriteContract::GetSimulateTxPin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::SimulateTx);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_ContractWriteContract::GetDataPin() const
{
	UEdGraphPin* Pin = FindPin(TwPins::Data);
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

#undef LOCTEXT_NAMESPACE
