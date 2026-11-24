// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewModels/AccessibilitySettingsViewModel.h"

#include "ViewModels/OptionSettingViewModel.h"
#include "ViewModels/ToggleSettingViewModel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AccessibilitySettingsViewModel)

#define LOCTEXT_NAMESPACE "Settings"

void UAccessibilitySettingsViewModel::Initialize()
{
	SetDisplayName(LOCTEXT("AccessibilityTabDisplayName", "Accessibility"));
	SetTabNameId("AccessibilitySettings");

	// TODO: Read settings from save object instead of hardcoding defaults
	TextSize = CreateSetting<UOptionSettingViewModel>(LOCTEXT("TextSize", "Text Size"));
	TextSize->Configure(
		{
			LOCTEXT("TextSizeSmall", "Small"),
			LOCTEXT("TextSizeDefault", "Default"),
			LOCTEXT("TextSizeLarge", "Large"),
			LOCTEXT("TextSizeExtraLarge", "Extra Large")
		},
		1);

	Subtitles = CreateSetting<UToggleSettingViewModel>(LOCTEXT("Subtitles", "Subtitles"));
	Subtitles->Configure(true);

	SubtitleBackground = CreateSetting<UToggleSettingViewModel>(LOCTEXT("SubtitleBackground", "Subtitle Background"));
	SubtitleBackground->Configure(true);

	ColorblindMode = CreateSetting<UOptionSettingViewModel>(LOCTEXT("ColorblindMode", "Colorblind Mode"));
	ColorblindMode->Configure(
		{
			LOCTEXT("ColorblindOff", "Off"),
			LOCTEXT("ColorblindDeuteranopia", "Deuteranopia"),
			LOCTEXT("ColorblindProtanopia", "Protanopia"),
			LOCTEXT("ColorblindTritanopia", "Tritanopia")
		},
		0);

	Subtitles->OnValueChanged.AddUObject(this, &ThisClass::HandleSubtitlesChanged);
	HandleSubtitlesChanged(Subtitles);
}

void UAccessibilitySettingsViewModel::HandleSubtitlesChanged(USettingViewModelBase* Setting)
{
	SubtitleBackground->SetIsEditable(Subtitles->GetValue());
}

#undef LOCTEXT_NAMESPACE
