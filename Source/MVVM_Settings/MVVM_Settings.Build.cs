// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class MVVM_Settings : ModuleRules
{
	public MVVM_Settings(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Lets any file include headers by module-relative path, e.g. "ViewModels/SettingsViewModel.h".
		PublicIncludePaths.Add(ModuleDirectory);

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",

			// UI
			"UMG",
			"CommonUI",
			"CommonInput",

			// MVVM: viewmodel base class and FieldNotify support
			"ModelViewViewModel",
			"FieldNotification"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
