// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebLoginWithSiwe.h"

#include "Async/TaskGraphInterfaces.h"
#include "Blueprint/UserWidget.h"
#include "Browser/ThirdwebOAuthBrowserUserWidget.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "ThirdwebLog.h"

void UAsyncTaskThirdwebLoginWithSiwe::Activate() {
  Browser->OnSiweComplete.AddDynamic(this, &ThisClass::HandleSiweComplete);
  Browser->OnError.AddDynamic(this, &ThisClass::HandleFailed);
  Browser->AddToViewport(10000);
  Browser->Authenticate(Wallet);
}

UAsyncTaskThirdwebLoginWithSiwe *UAsyncTaskThirdwebLoginWithSiwe::LoginWithSiwe(
    UObject *WorldContextObject, const FInAppWalletHandle &Wallet) {
  if (!WorldContextObject) {
    return nullptr;
  }
  NEW_TASK
  Task->Wallet = Wallet;
  Task->Browser = CreateWidget<UThirdwebOAuthBrowserUserWidget>(
      UGameplayStatics::GetGameInstance(WorldContextObject),
      UThirdwebOAuthBrowserUserWidget::StaticClass());
  Task->RegisterWithGameInstance(WorldContextObject);
  return Task;
}

void UAsyncTaskThirdwebLoginWithSiwe::HandleSiweComplete(
    const FString &Payload, const FString &Signature) {
  if (IsInGameThread()) {
    Success.Broadcast(Payload, Signature);
    Browser->RemoveFromParent();
    SetReadyToDestroy();
  } else {
    // Retry on the GameThread.
    TWeakObjectPtr<UAsyncTaskThirdwebLoginWithSiwe> WeakThis = this;
    FFunctionGraphTask::CreateAndDispatchWhenReady(
        [WeakThis, Payload, Signature]() {
          if (WeakThis.IsValid()) {
            WeakThis->HandleSiweComplete(Payload, Signature);
          }
        },
        TStatId(), nullptr, ENamedThreads::GameThread);
  }
}

void UAsyncTaskThirdwebLoginWithSiwe::HandleFailed(const FString &Error) {
  if (IsInGameThread()) {
    Browser->RemoveFromParent();
    Failed.Broadcast(Error);
    SetReadyToDestroy();
  } else {
    // Retry on the GameThread.
    TWeakObjectPtr<UAsyncTaskThirdwebLoginWithSiwe> WeakThis = this;
    FFunctionGraphTask::CreateAndDispatchWhenReady(
        [WeakThis, Error]() {
          if (WeakThis.IsValid()) {
            WeakThis->HandleFailed(Error);
          }
        },
        TStatId(), nullptr, ENamedThreads::GameThread);
  }
}
