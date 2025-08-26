// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class tpindivJuanCabral : ModuleRules
{
	public tpindivJuanCabral(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"tpindivJuanCabral",
			"tpindivJuanCabral/Variant_Platforming",
			"tpindivJuanCabral/Variant_Platforming/Animation",
			"tpindivJuanCabral/Variant_Combat",
			"tpindivJuanCabral/Variant_Combat/AI",
			"tpindivJuanCabral/Variant_Combat/Animation",
			"tpindivJuanCabral/Variant_Combat/Gameplay",
			"tpindivJuanCabral/Variant_Combat/Interfaces",
			"tpindivJuanCabral/Variant_Combat/UI",
			"tpindivJuanCabral/Variant_SideScrolling",
			"tpindivJuanCabral/Variant_SideScrolling/AI",
			"tpindivJuanCabral/Variant_SideScrolling/Gameplay",
			"tpindivJuanCabral/Variant_SideScrolling/Interfaces",
			"tpindivJuanCabral/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
