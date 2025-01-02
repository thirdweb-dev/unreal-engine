// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/EnglishAuctions/AsyncTaskEnglishAuctionsIsWinningBid.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"

UAsyncTaskEnglishAuctionsIsWinningBid* UAsyncTaskEnglishAuctionsIsWinningBid::IsWinningBid(
	UObject* WorldContextObject,
	const int64 ChainId,
	const FString& Contract,
	const FString& ListingId,
	const FString& BidAmount
)
{
	NEW_TASK
	Task->Chain = ChainId;
	Task->MarketplaceContract = Contract;
	Task->ListingId = ListingId;
	Task->BidAmount = BidAmount;
	RR_TASK
}

UAsyncTaskEnglishAuctionsIsWinningBid* UAsyncTaskEnglishAuctionsIsWinningBid::MarketplaceIsWinningBid(
	UObject* WorldContextObject,
	const UThirdwebMarketplace* Marketplace,
	const FString& ListingId,
	const FString& BidAmount
)
{
	NEW_MARKETPLACE_TASK
	Task->Chain = Marketplace->GetChainId();
	Task->MarketplaceContract = Marketplace->GetContractAddress();
	Task->ListingId = ListingId;
	Task->BidAmount = BidAmount;
	RR_TASK
}

void UAsyncTaskEnglishAuctionsIsWinningBid::HandleResponse(const bool bApproved)
{
	Success.Broadcast(bApproved, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskEnglishAuctionsIsWinningBid::HandleFailed(const FString& Error)
{
	Failed.Broadcast(false, Error);
	SetReadyToDestroy();
}

void UAsyncTaskEnglishAuctionsIsWinningBid::Activate()
{
	ThirdwebEngine::Marketplace::EnglishAuctions::IsWinningBid(
		this,
		ListingId,
		BidAmount,
		Chain,
		MarketplaceContract,
		BIND_UOBJECT_DELEGATE(FBoolDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
