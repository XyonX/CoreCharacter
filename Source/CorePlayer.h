#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

// Declare your module class
class FCorePlayerModule : public IModuleInterface
{
public:
    // IModuleInterface implementation
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
