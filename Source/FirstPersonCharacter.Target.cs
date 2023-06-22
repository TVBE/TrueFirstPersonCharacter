// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

using UnrealBuildTool;
using System.Collections.Generic;

public class FirstPersonCharacterTarget : TargetRules
{
	public FirstPersonCharacterTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "FirstPersonCharacter" } );
	}
}
