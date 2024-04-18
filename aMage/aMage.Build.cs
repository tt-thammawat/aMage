// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class aMage : ModuleRules
{
	public aMage(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject",
			"Engine", 
			"InputCore",
			"EnhancedInput",
			"UMG",
			"GameplayAbilities",
			"OnlineSubsystemSteam", 
			"OnlineSubsystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"GameplayTags",
			"GameplayTasks",
			"Slate",
			"SlateCore", 
			"Niagara",
			"AIModule", "WebRTC"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
