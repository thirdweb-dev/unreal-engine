// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#pragma once

#include "K2Node.h"
#include "K2Node/K2Node_ThirdwebBaseAsyncTask.h"
#include "K2Node_ContractWriteContract.generated.h"

namespace TwPins
{
	extern const FName ChainId;
	extern const FName ContractAddress;
	extern const FName BackendWalletAddress;
	extern const FName IdempotencyKey;
	extern const FName FunctionName;
	extern const FName Args;
	extern const FName TxOverrides;
	extern const FName Abi;
	extern const FName SimulateTx;
	extern const FName QueueId;
}

UCLASS()
class THIRDWEBUNCOOKEDONLY_API UK2Node_ContractWriteContract : public UK2Node_ThirdwebBaseAsyncTask
{
	GENERATED_BODY()

public:
	UK2Node_ContractWriteContract();
	
	// UEdGraphNode interface implementation
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void AllocateDefaultPins() override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	// End of implementation

	// UK2Node interface implementation
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual FText GetMenuCategory() const override;
	virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	virtual bool ShouldShowNodeProperties() const override { return true; }
	// End of implementation

protected:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	bool bRaw;
	
	virtual bool UpdatePins() override;

	/** Pin Getters */
	virtual UEdGraphPin* GetChainIdPin() const;
	virtual UEdGraphPin* GetContractAddressPin() const;
	virtual UEdGraphPin* GetBackendWalletAddressPin() const;
	virtual UEdGraphPin* GetIdempotencyKeyPin() const;
	virtual UEdGraphPin* GetFunctionNamePin() const;
	virtual UEdGraphPin* GetArgsPin() const;
	virtual UEdGraphPin* GetTxOverridesPin() const;
	virtual UEdGraphPin* GetAbiPin() const;
	virtual UEdGraphPin* GetSimulateTxPin() const;
	virtual UEdGraphPin* GetDataPin() const;
};
