// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

using UnrealBuildTool;

public class FirstPersonCharacter : ModuleRules
{
	public FirstPersonCharacter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "PhysicsCore", "Niagara", "Reacoustic", "Synthesis", "Chaos" });

		PrivateDependencyModuleNames.AddRange(new string[] { "RiderLink", "MetasoundEngine", "AnimGraphRuntime"});
		
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.Add("UnrealEd");
		}
		
		PublicDefinitions.Add("WITH_REACOUSTIC=1");

		PublicIncludePaths.Add("$(ProjectDir)/Source/FirstPersonCharacter/Actors/Public");
		PublicIncludePaths.Add("$(ProjectDir)/Source/FirstPersonCharacter/Core/Public");
		PublicIncludePaths.Add("$(ProjectDir)/Source/FirstPersonCharacter/Interfaces/Public");
		PublicIncludePaths.Add("$(ProjectDir)/Source/FirstPersonCharacter/Libraries/Public");
		PublicIncludePaths.Add("$(ProjectDir)/Source/FirstPersonCharacter/PlayerCharacter/Public");
		PublicIncludePaths.Add("$(ProjectDir)/Source/FirstPersonCharacter/ActorComponents/Public");
	}
}
