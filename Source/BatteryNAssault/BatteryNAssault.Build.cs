// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BatteryNAssault : ModuleRules
{
	public BatteryNAssault(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemSteam", "OnlineSubsystemNull" });
	}
}
