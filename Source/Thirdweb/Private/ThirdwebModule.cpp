// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#include "ThirdwebModule.h"
#include "ThirdwebAssetManager.h"
#include "ThirdwebLog.h"

class FThirdwebModule : public IThirdwebModule {
public:
  virtual void StartupModule() override {
    if (ThirdwebAssetManager == nullptr) {
      ThirdwebAssetManager =
          NewObject<UThirdwebAssetManager>((UObject *)GetTransientPackage(),
                                           NAME_None, RF_Transient | RF_Public);
      ThirdwebAssetManager->LoadDefaultMaterials();
    }
  }

  virtual void ShutdownModule() override {}

private:
  UThirdwebAssetManager *ThirdwebAssetManager = nullptr;
};

IMPLEMENT_MODULE(FThirdwebModule, Thirdweb);