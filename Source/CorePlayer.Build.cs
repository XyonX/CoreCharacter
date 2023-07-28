using UnrealBuildTool;

public class CorePlayer : ModuleRules
{
    public CorePlayer(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        // Add any other dependencies or settings here if needed
    }
}
