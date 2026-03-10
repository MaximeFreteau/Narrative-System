// Copyright Maxime Freteau. All Rights Reserved.

using UnrealBuildTool;

public class NarrativeSystemEditor : ModuleRules
{
	public NarrativeSystemEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"NarrativeSystem"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"AssetTools",
				"UnrealEd",
				"Slate",
				"SlateCore",
				"EditorStyle",
				"GraphEditor",
				"BlueprintGraph",
				"KismetCompiler",
				"PropertyEditor",
				"ToolMenus",
				"ApplicationCore",
				"InputCore",
				"GameplayTags",
				"AssetDefinition",
				"EditorFramework",
				"WorkspaceMenuStructure"
			}
		);
	}
}