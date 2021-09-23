// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GhostInTheGraveyardEditor : ModuleRules
{
	public GhostInTheGraveyardEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "AppFramework",
            "SlateCore",
            "AnimGraph",
            "UnrealEd",
            "KismetWidgets",
            "MainFrame",
            "PropertyEditor",
            "ComponentVisualizers",
            "GhostInTheGraveyard",
            "GameplayDebugger",
            "InputCore",
            "LevelEditor",
            "Slate",
            "EditorStyle",
            "AssetTools",
            "EditorWidgets",
            "BlueprintGraph"
        });

//        PublicIncludePaths.AddRange(new string[] { "GhostInTheGraveyardEditor/Public" });
//        PrivateIncludePaths.AddRange(new string[] { "GhostInTheGraveyardEditor/Private" });
    }
}
