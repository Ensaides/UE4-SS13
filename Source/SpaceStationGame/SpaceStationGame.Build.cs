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
		        "SpaceStationGame/Mobs",
                "SpaceStationGame/Jobs",
                "SpaceStationGame/Items",
                "SpaceStationGame/Items/ID",
                "SpaceStationGame/Items/InstancedItems",
                "SpaceStationGame/Items/InstancedItems/TiledItems",
		        "SpaceStationGame/Items/ReagentContainers",
				"SpaceStationGame/RoundTypes",
                "SpaceStationGame/Voxel",
                "SpaceStationGame/OpenCL",
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

        LoadMySQL(Target);

        LoadBoost(Target);

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
    }

    public bool LoadMySQL(TargetInfo Target)
    {
        bool isLibrarySupported = false;

        if (((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32)) && ((Target.Type == TargetRules.TargetType.Server) || (Target.Type == TargetRules.TargetType.Editor)))
        {
            isLibrarySupported = true;

            //string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string IncludesPath = Path.Combine(ThirdPartyPath, "MySQL", "Includes");
            string LibrariesPath = Path.Combine(ThirdPartyPath, "MySQL", "Libraries");

            if (Target.Platform == UnrealTargetPlatform.Win64)
            {
                LibrariesPath = Path.Combine(LibrariesPath, "x64");
            }
            else if (Target.Platform == UnrealTargetPlatform.Win32)
            {
                LibrariesPath = Path.Combine(LibrariesPath, "Win32");
            }

            PublicIncludePaths.Add(IncludesPath);
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "mysqlcppconn.lib"));
        }

        //if (isLibrarySupported)
        //{
        //    // Include path
        //    PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "MySQL", "Includes"));
        //}

        Definitions.Add(string.Format("WITH_MYSQL_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }

    public bool LoadBoost(TargetInfo Target)
    {
        bool isLibrarySupported = false;

        if (((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32)) && ((Target.Type == TargetRules.TargetType.Server) || (Target.Type == TargetRules.TargetType.Editor)))
        {
            isLibrarySupported = true;

            //string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            //string LibrariesPath = Path.Combine(ThirdPartyPath, "MySQL", "Libraries");

            //PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "mysqlcppconn-static.lib"));
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "Boost", "Includes"));
        }

        Definitions.Add(string.Format("WITH_BOOST_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }
}