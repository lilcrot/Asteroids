// A test task by KEFIR

using UnrealBuildTool;

public class Asteroids : ModuleRules
{
    public Asteroids(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
            "AIModule", "GameplayTasks", "AudioMixer",  "UMG", "Json", "JsonUtilities"
        });

        if (Target.Configuration != UnrealTargetConfiguration.Shipping)
        {
            PublicDependencyModuleNames.Add("FunctionalTesting");
        }
    }
}
