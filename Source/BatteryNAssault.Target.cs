// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BatteryNAssaultTarget : TargetRules
{
	public BatteryNAssaultTarget(TargetInfo Target)
	{
		Type = TargetType.Game;
	}

	//
	// TargetRules interface.
	//

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.Add("BatteryNAssault");
	}
}
