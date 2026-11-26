// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewModels/ControlsSettingsViewModel.h"

#include "Settings/SettingsGameUserSettings.h"
#include "ViewModels/SliderSettingViewModel.h"
#include "ViewModels/ToggleSettingViewModel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ControlsSettingsViewModel)

#define LOCTEXT_NAMESPACE "Settings"

void UControlsSettingsViewModel::Initialize()
{
	SetDisplayName(LOCTEXT("ControlsTabDisplayName", "Controls"));
	SetTabNameId("ControlsSettings");

	LookSensitivity = CreateSetting<USliderSettingViewModel>(LOCTEXT("LookSensitivity", "Look Sensitivity"));
	LookSensitivity->Configure(0.1f, 5.0f, 0.1f, 1.0f, ESliderValueFormat::Decimal);

	InvertY = CreateSetting<UToggleSettingViewModel>(LOCTEXT("InvertY", "Invert Y Axis"));
	InvertY->Configure(false);

	Vibration = CreateSetting<UToggleSettingViewModel>(LOCTEXT("Vibration", "Vibration"));
	Vibration->Configure(true);
}

void UControlsSettingsViewModel::LoadFromSettings(const USettingsGameUserSettings& UserSettings)
{
	LookSensitivity->SetValue(UserSettings.LookSensitivity);
	InvertY->SetValue(UserSettings.bInvertY);
	Vibration->SetValue(UserSettings.bVibration);
}

void UControlsSettingsViewModel::SaveToSettings(USettingsGameUserSettings& UserSettings) const
{
	UserSettings.LookSensitivity = LookSensitivity->GetValue();
	UserSettings.bInvertY = InvertY->GetValue();
	UserSettings.bVibration = Vibration->GetValue();
}

void UControlsSettingsViewModel::SetUsingGamepad(bool bUsingGamepad)
{
	// Vibration is only editable while a gamepad is in use
	Vibration->SetIsEditable(bUsingGamepad);
}

#undef LOCTEXT_NAMESPACE
