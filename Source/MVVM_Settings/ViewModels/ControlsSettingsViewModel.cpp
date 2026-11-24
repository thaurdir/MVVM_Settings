// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewModels/ControlsSettingsViewModel.h"

#include "ViewModels/SliderSettingViewModel.h"
#include "ViewModels/ToggleSettingViewModel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ControlsSettingsViewModel)

#define LOCTEXT_NAMESPACE "Settings"

void UControlsSettingsViewModel::Initialize()
{
	SetDisplayName(LOCTEXT("ControlsTabDisplayName", "Controls"));
	SetTabNameId("ControlsSettings");

	// TODO: Read settings from save object instead of hardcoding defaults
	LookSensitivity = CreateSetting<USliderSettingViewModel>(LOCTEXT("LookSensitivity", "Look Sensitivity"));
	LookSensitivity->Configure(0.1f, 5.0f, 0.1f, 1.0f, ESliderValueFormat::Decimal);

	InvertY = CreateSetting<UToggleSettingViewModel>(LOCTEXT("InvertY", "Invert Y Axis"));
	InvertY->Configure(false);

	Vibration = CreateSetting<UToggleSettingViewModel>(LOCTEXT("Vibration", "Vibration"));
	Vibration->Configure(true);
}

#undef LOCTEXT_NAMESPACE
