// Copyright Maxime Freteau. All Rights Reserved.

using UnrealBuildTool;

public class NarrativeSystem : ModuleRules
{
	public NarrativeSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"GameplayTags",
				"DeveloperSettings"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Json",
				"JsonUtilities",
				"Slate",
				"SlateCore"
			}
		);
	}
}