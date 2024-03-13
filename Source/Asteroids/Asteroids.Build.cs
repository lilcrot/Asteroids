// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Asteroids : ModuleRules
{
    public Asteroids(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        { 
            "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
            "AIModule", "GameplayTasks", "AudioMixer"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
