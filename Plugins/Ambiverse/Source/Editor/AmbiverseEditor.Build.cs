// Some copyright should be here...

using UnrealBuildTool;

public class AmbiverseEditor : ModuleRules
{
	public AmbiverseEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
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
				"AssetTools",
				"Ambiverse"
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
				"Settings",
				"UnrealEd",
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
