// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskMarketplacePostBase.h"
#include "Engine/Marketplace/DirectListings/ThirdwebMarketplaceDirectListing.h"
#include "AsyncTaskDirectListingsRevokeReservedBuyerApproval.generated.h"

class UThirdwebMarketplace;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskDirectListingsRevokeReservedBuyerApproval : public UAsyncTaskMarketplacePostBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Revoke Buyer Approval for Reserved Direct Listing",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="IdempotencyKey,bSimulateTx"),
		Category="Thirdweb|Engine|Marketplace|Direct Listings|Write|Raw")
	static UAsyncTaskDirectListingsRevokeReservedBuyerApproval* RevokeReservedBuyerApproval(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		const FString& BackendWallet,
		const FSmartWalletHandle& Account,
		UPARAM(DisplayName="Listing ID") const FString& ListingId,
		const FString& Buyer,
		const FString& IdempotencyKey,
		UPARAM(DisplayName="Simulate TX") const bool bSimulateTx
	);

	UFUNCTION(BlueprintCallable, DisplayName="Revoke Buyer Approval for Reserved Direct Listing",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="IdempotencyKey,bSimulateTx"),
		Category="Thirdweb|Engine|Marketplace|Direct Listings|Write")
	static UAsyncTaskDirectListingsRevokeReservedBuyerApproval* MarketplaceRevokeReservedBuyerApproval(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		const FString& BackendWallet,
		const FSmartWalletHandle& Account,
		UPARAM(DisplayName="Listing ID") const FString& ListingId,
		const FString& Buyer,
		const FString& IdempotencyKey,
		UPARAM(DisplayName="Simulate TX") const bool bSimulateTx
	);

protected:
	UPROPERTY(Transient)
	FString ListingId;

	UPROPERTY(Transient)
	FString Buyer;

public:
	virtual void Activate() override;
};
