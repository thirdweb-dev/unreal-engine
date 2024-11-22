// AssetCurrencyValue.h
#pragma once

#include "ThirdwebMarketplaceListing.generated.h"

class FJsonObject;
class FJsonValue;
enum class EThirdwebMarketplaceListingStatus : uint8;

USTRUCT(BlueprintType)
struct FThirdwebAssetCurrencyValue
{
	GENERATED_BODY()
    
	UPROPERTY(BlueprintReadWrite, Category="Currency Value")
	FString Name;

	UPROPERTY(BlueprintReadWrite, Category="Currency Value")
	FString Symbol;

	UPROPERTY(BlueprintReadWrite, Category="Currency Value")
	int32 Decimals = 0;

	UPROPERTY(BlueprintReadWrite, Category="Currency Value")
	FString Value;

	UPROPERTY(BlueprintReadWrite, Category="Currency Value")
	FString DisplayValue;

	static FThirdwebAssetCurrencyValue FromJson(const TSharedPtr<FJsonObject>& JsonObject);
};


USTRUCT(BlueprintType)
struct THIRDWEB_API FThirdwebAssetAttribute
{
	GENERATED_BODY()
    
	UPROPERTY(BlueprintReadWrite, Category="Asset Attribute")
	FString TraitType;

	UPROPERTY(BlueprintReadWrite, Category="Asset Attribute")
	FString Value;

	static FThirdwebAssetAttribute FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static TArray<FThirdwebAssetAttribute> FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray);
};


USTRUCT(BlueprintType)
struct THIRDWEB_API FThirdwebAsset
{
	GENERATED_BODY()
    
	UPROPERTY(BlueprintReadWrite, Category="Asset")
	FString Id;

	UPROPERTY(BlueprintReadWrite, Category="Asset")
	FString Uri;

	UPROPERTY(BlueprintReadWrite, Category="Asset")
	FString Name;

	UPROPERTY(BlueprintReadWrite, Category="Asset")
	FString Description;

	UPROPERTY(BlueprintReadWrite, Category="Asset")
	FString ExternalUrl;

	UPROPERTY(BlueprintReadWrite, Category="Asset")
	TArray<FThirdwebAssetAttribute> Attributes;

	static FThirdwebAsset FromJson(const TSharedPtr<FJsonObject>& JsonObject);
};

USTRUCT()
struct FThirdwebMarketplaceBaseAuction
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="Core")
	FString AssetContractAddress;

	UPROPERTY(BlueprintReadWrite, Category="Core")
	FString TokenId;

	UPROPERTY(BlueprintReadWrite, Category="Core")
	FString CurrencyContractAddress;

	UPROPERTY(BlueprintReadWrite, Category="Core")
	FString Quantity;

	UPROPERTY(BlueprintReadWrite, Category="Core")
	FString Id;

	UPROPERTY(BlueprintReadWrite, Category="Core")
	FThirdwebAsset Asset;

	UPROPERTY(BlueprintReadWrite, Category="Core")
	EThirdwebMarketplaceListingStatus Status;

	UPROPERTY(BlueprintReadWrite, Category="Core")
	FDateTime StartTimeInSeconds;

	UPROPERTY(BlueprintReadWrite, Category="Core")
	FDateTime EndTimeInSeconds;
	
	static FThirdwebMarketplaceBaseAuction FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static TArray<FThirdwebMarketplaceBaseAuction> FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray);
};

USTRUCT(BlueprintType, DisplayName="Marketplace Listing")
struct THIRDWEB_API FThirdwebMarketplaceListing : public FThirdwebMarketplaceBaseAuction
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category="Listing")
	FString PricePerToken;

	UPROPERTY(BlueprintReadWrite, Category="Listing")
	bool bIsReservedListing = false;
	
	UPROPERTY(BlueprintReadWrite, Category="Listing")
	FThirdwebAssetCurrencyValue CurrencyValuePerToken;

	static FThirdwebMarketplaceListing FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static TArray<FThirdwebMarketplaceListing> FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray);
};

USTRUCT(BlueprintType, DisplayName="Marketplace English Auction")
struct THIRDWEB_API FThirdwebMarketplaceEnglishAuction : public FThirdwebMarketplaceBaseAuction
{
	GENERATED_BODY()

		
	UPROPERTY(BlueprintReadWrite, Category="Auction")
	FString MinimumBidAmount;

	UPROPERTY(BlueprintReadWrite, Category="Auction")
	FString BuyoutBidAmount;
	
	UPROPERTY(BlueprintReadWrite, Category="Auction")
	FThirdwebAssetCurrencyValue BuyoutCurrencyValue;

	UPROPERTY(BlueprintReadWrite, Category="Auction")
	FDateTime TimeBufferInSeconds;

	UPROPERTY(BlueprintReadWrite, Category="Auction")
	int32 BidBufferBps;
	
	static FThirdwebMarketplaceEnglishAuction FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static TArray<FThirdwebMarketplaceEnglishAuction> FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray);
};

USTRUCT(BlueprintType, DisplayName="Marketplace Offer")
struct THIRDWEB_API FThirdwebMarketplaceOffer : public FThirdwebMarketplaceBaseAuction
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category="Offer")
	FString OfferorAddress;
	
	UPROPERTY(BlueprintReadWrite, Category="Offer")
	FThirdwebAssetCurrencyValue CurrencyValue;

	UPROPERTY(BlueprintReadWrite, Category="Offer")
	FString TotalPrice;
	
	static FThirdwebMarketplaceOffer FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static TArray<FThirdwebMarketplaceOffer> FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray);
};

USTRUCT(BlueprintType, DisplayName="Marketplace Bid")
struct THIRDWEB_API FThirdwebMarketplaceBid
{
	GENERATED_BODY()

		
	UPROPERTY(BlueprintReadWrite, Category="Bid")
	FString AuctionId;
	
	UPROPERTY(BlueprintReadWrite, Category="Bid")
	FString BidderAddress;

	UPROPERTY(BlueprintReadWrite, Category="Bid")
	FString CurrencyContractAddress;

	UPROPERTY(BlueprintReadWrite, Category="Bid")
	FString BidAmount;

	UPROPERTY(BlueprintReadWrite, Category="Bid")
	FThirdwebAssetCurrencyValue BidAmountCurrencyValue;
	
	static FThirdwebMarketplaceBid FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static TArray<FThirdwebMarketplaceBid> FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray);
};
