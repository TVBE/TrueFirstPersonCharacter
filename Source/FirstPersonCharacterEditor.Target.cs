// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

using UnrealBuildTool;
using System.Collections.Generic;

public class FirstPersonCharacterEditorTarget : TargetRules
{
	public FirstPersonCharacterEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "FirstPersonCharacter" } );
	}
}
