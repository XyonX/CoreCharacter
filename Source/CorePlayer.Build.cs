using UnrealBuildTool;

public class CorePlayer : ModuleRules
{
    public CorePlayer(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine","EnhancedInput","InputCore", "OnlineSubsystem",});

        PrivateDependencyModuleNames.AddRange(new string[] { "ModularGameplay", "CoreAbilitySystem" });

        // Add any other dependencies or settings here if needed
    }
}
