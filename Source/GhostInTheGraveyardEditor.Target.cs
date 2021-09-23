// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GhostInTheGraveyardEditorTarget : TargetRules
{
	public GhostInTheGraveyardEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("GhostInTheGraveyard");
		ExtraModuleNames.AddRange(new string[] { "GhostInTheGraveyardEditor" });
	}
}
