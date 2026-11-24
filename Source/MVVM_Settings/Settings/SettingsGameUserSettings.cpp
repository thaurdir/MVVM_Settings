// Fill out your copyright notice in the Description page of Project Settings.

#include "Settings/SettingsGameUserSettings.h"

#include "Engine/Engine.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SettingsGameUserSettings)

USettingsGameUserSettings::USettingsGameUserSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ResetProjectSettings();
}

USettingsGameUserSettings* USettingsGameUserSettings::Get()
{
	return GEngine ? Cast<USettingsGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

void USettingsGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();

	// The engine's SetToDefaults leaves VSync as it was.
	SetVSyncEnabled(false);
	ResetProjectSettings();
}

void USettingsGameUserSettings::ResetProjectSettings()
{
	MasterVolume = 1.0f;
	MusicVolume = 1.0f;
	EffectsVolume = 1.0f;
	DialogueVolume = 1.0f;

	LookSensitivity = 1.0f;
	bInvertY = false;
	bVibration = true;

	TextSize = ESettingsTextSize::Default;
	bSubtitles = true;
	bSubtitleBackground = true;
	ColorblindMode = ESettingsColorblindMode::Off;
}
