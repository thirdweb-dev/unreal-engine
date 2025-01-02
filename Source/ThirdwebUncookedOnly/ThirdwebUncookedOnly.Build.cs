﻿// Copyright (c) 2025 Thirdweb. All Rights Reserved.

using UnrealBuildTool;

public class ThirdwebUncookedOnly : ModuleRules
{
	public ThirdwebUncookedOnly(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
#if UE_5_3_OR_LATER
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
#endif
		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"Thirdweb",
			"DeveloperSettings",
			"KismetCompiler",
			"BlueprintGraph",
			"UnrealEd",
			"JsonUtilities"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore"
		});
	}
}