// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class SpaceStationGame : ModuleRules
{
    private string ModulePath
    {
        get { return Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name)); }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }

    public SpaceStationGame(TargetInfo Target)
    {
        {
            PrivateIncludePaths.AddRange(
                new string[] { 
		        "SpaceStationGame",
				"SpaceStationGame/Atmospherics",
				"SpaceStationGame/Atmospherics/OpenCL",
		        "SpaceStationGame/Mobs",
				"SpaceStationGame/Player",
                "SpaceStationGame/Jobs",
                "SpaceStationGame/Items",
                "SpaceStationGame/Items/ID",
                "SpaceStationGame/Items/InstancedItems",
                "SpaceStationGame/Items/InstancedItems/TiledItems",
		        "SpaceStationGame/Items/ReagentContainers",
				"SpaceStationGame/RoundTypes",
                "SpaceStationGame/MySQL",
		        "SpaceStationGame/Items/Machines"
                }
            );

            PublicDependencyModuleNames.AddRange(
                new string[] { 
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "Slate",
                "SlateCore",
                "XmlParser",
                "OpenCL",
                "OnlineSubsystem",
            	"OnlineSubsystemUtils"
                }
            );
        }

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");

        LoadOTL(Target);
    }

    public bool LoadOTL(TargetInfo Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Type == TargetRules.TargetType.Server) || (Target.Type == TargetRules.TargetType.Editor))
        {
            isLibrarySupported = true;

            string IncludesPath = Path.Combine(ThirdPartyPath, "OTL", "Includes");

            PublicIncludePaths.Add(IncludesPath);
        }

        Definitions.Add(string.Format("WITH_OTL_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }
}