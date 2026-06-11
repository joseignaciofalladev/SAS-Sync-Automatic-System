using UnrealBuildTool;

public class SAS : ModuleRules
{
    public SAS(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp17;

        PublicIncludePaths.AddRange(new string[]{"SAS/Public"});
        PrivateIncludePaths.AddRange(new string[]{"SAS/Private"});

        /////////////////////////////////////
        // PUBLIC DEPENDENCIES
        /////////////////////////////////////
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "GameplayTags",
                "DeveloperSettings"
            }
        );

        /////////////////////////////////////
        // PRIVATE DEPENDENCIES
        /////////////////////////////////////
        PrivateDependencyModuleNames.AddRange(new string[]{});

        /////////////////////////////////////
        // UE4.26 COMPATIBILITY
        /////////////////////////////////////
        bUseUnity = true;
        bEnableExceptions = false;

        /////////////////////////////////////
        // EDITOR SUPPORT
        /////////////////////////////////////
        if (Target.bBuildEditor){
            PrivateDependencyModuleNames.AddRange(new string[]{"UnrealEd"});
        }
    }
}
