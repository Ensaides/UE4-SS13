// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SpaceStationGame : ModuleRules
{
	public SpaceStationGame(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core",
		"CoreUObject",
		"Engine",
        "Slate",
        "SlateCore",
		"OpenCL",
        "UMG",
		"InputCore" });

        PrivateIncludePaths.AddRange(
                new string[] {
                "SpaceStationGame",
                "SpaceStationGame/Atmospherics",
                "SpaceStationGame/Game",
                "SpaceStationGame/Pawns",
                "SpaceStationGame/Player",
                "SpaceStationGame/OpenCL",
                "SpaceStationGame/Jobs",
                "SpaceStationGame/Items",
                "SpaceStationGame/Items/Instances",
                "SpaceStationGame/Managers",
                "SpaceStationGame/Helpers",
                "SpaceStationGame/Structs"
                }
            );
			
		DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
    }
}
