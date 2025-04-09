// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GAS_Slice : ModuleRules
{
	public GAS_Slice(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "CableComponent","PhysicsCore" });
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CableComponent", "ClothingSystemRuntimeCommon", "AIModule", "GameplayAbilities"
			,"GameplayTags", "GameplayTasks"
		});
	}
}
