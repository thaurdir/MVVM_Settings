// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewModels/AudioSettingsViewModel.h"

#include "HAL/IConsoleManager.h"
#include "Settings/SettingsGameUserSettings.h"
#include "UObject/UObjectIterator.h"
#include "ViewModels/SliderSettingViewModel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AudioSettingsViewModel)

#define LOCTEXT_NAMESPACE "Settings"

void UAudioSettingsViewModel::Initialize()
{
	SetDisplayName(LOCTEXT("AudioTabDisplayName", "Audio"));
	SetTabNameId("AudioSettings");

	MasterVolume = CreateSetting<USliderSettingViewModel>(LOCTEXT("MasterVolume", "Master Volume"));
	MasterVolume->Configure(0.0f, 1.0f, 0.01f, 1.0f, ESliderValueFormat::Percent);

	MusicVolume = CreateSetting<USliderSettingViewModel>(LOCTEXT("MusicVolume", "Music Volume"));
	MusicVolume->Configure(0.0f, 1.0f, 0.01f, 1.0f, ESliderValueFormat::Percent);

	EffectsVolume = CreateSetting<USliderSettingViewModel>(LOCTEXT("EffectsVolume", "Effects Volume"));
	EffectsVolume->Configure(0.0f, 1.0f, 0.01f, 1.0f, ESliderValueFormat::Percent);

	DialogueVolume = CreateSetting<USliderSettingViewModel>(LOCTEXT("DialogueVolume", "Dialogue Volume"));
	DialogueVolume->Configure(0.0f, 1.0f, 0.01f, 1.0f, ESliderValueFormat::Percent);
}

void UAudioSettingsViewModel::LoadFromSettings(const USettingsGameUserSettings& UserSettings)
{
	MasterVolume->SetValue(UserSettings.MasterVolume);
	MusicVolume->SetValue(UserSettings.MusicVolume);
	EffectsVolume->SetValue(UserSettings.EffectsVolume);
	DialogueVolume->SetValue(UserSettings.DialogueVolume);
}

void UAudioSettingsViewModel::SaveToSettings(USettingsGameUserSettings& UserSettings) const
{
	// TODO: Apply the volumes to sound classes. For now they're only saved
	UserSettings.MasterVolume = MasterVolume->GetValue();
	UserSettings.MusicVolume = MusicVolume->GetValue();
	UserSettings.EffectsVolume = EffectsVolume->GetValue();
	UserSettings.DialogueVolume = DialogueVolume->GetValue();
}

#undef LOCTEXT_NAMESPACE

#if !UE_BUILD_SHIPPING
// Debug-only console command for testing the C++ -> UI direction
static FAutoConsoleCommand GSetMasterVolumeCommand(
	TEXT("Settings.Audio.SetMasterVolume"),
	TEXT("Sets MasterVolume (0-1) on every live UAudioSettingsViewModel. Usage: Settings.Audio.SetMasterVolume <value>"),
	FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
	{
		if (Args.Num() < 1)
		{
			return;
		}

		const float NewValue = FCString::Atof(*Args[0]);
		for (TObjectIterator<UAudioSettingsViewModel> It; It; ++It)
		{
			// Skip the class default object; only live instances are bound to widgets.
			if (!It->HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject) && It->GetMasterVolume())
			{
				It->GetMasterVolume()->SetValue(NewValue);
			}
		}
	}));
#endif
