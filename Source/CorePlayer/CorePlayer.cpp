#include "CorePlayer.h"

#define LOCTEXT_NAMESPACE "FCorePlayerModule"

void FCorePlayerModule::StartupModule()
{
    // This code will execute after your module is loaded into memory
    // You can add any initialization logic specific to your plugin here
}

void FCorePlayerModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module
    // You can add any cleanup logic specific to your plugin here
}

#undef LOCTEXT_NAMESPACE

// Bind the module to the plugin name
IMPLEMENT_MODULE(FCorePlayerModule, CorePlayer)
