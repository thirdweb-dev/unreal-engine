// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FThirdwebModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
