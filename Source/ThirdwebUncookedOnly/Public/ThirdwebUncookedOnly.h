// Copyright (c) 2025 Thirdweb. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FThirdwebUncookedOnlyModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
