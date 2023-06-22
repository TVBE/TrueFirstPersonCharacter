// Some copyright should be here...

using UnrealBuildTool;

public class Ambiverse : ModuleRules
{
	public Ambiverse(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"$(ProjectDir)/Plugins/Ambiverse/Source/Runtime/Core/Public",
				"$(ProjectDir)/Plugins/Ambiverse/Source/Runtime/DataTypes/Public",
				"$(ProjectDir)/Plugins/Ambiverse/Source/Runtime/AssetTypes/Public"
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"MetasoundEngine",
			}
			);
		
		PublicIncludePathModuleNames.AddRange(new string[] { "AmbiverseEditor" });
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
