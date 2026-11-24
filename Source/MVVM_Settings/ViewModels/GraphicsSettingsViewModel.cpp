// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewModels/GraphicsSettingsViewModel.h"

#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Settings/SettingsGameUserSettings.h"
#include "ViewModels/OptionSettingViewModel.h"
#include "ViewModels/ToggleSettingViewModel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GraphicsSettingsViewModel)

#define LOCTEXT_NAMESPACE "Settings"

namespace GraphicsSettings
{
	// Scalability levels 0-3. UGameUserSettings also has 4 (Cinematic), which is clamped to Epic here.
	constexpr int32 MaxQualityLevel = 3;

	TArray<FText> GetQualityLevelOptions()
	{
		return {
			LOCTEXT("QualityLow", "Low"),
			LOCTEXT("QualityMedium", "Medium"),
			LOCTEXT("QualityHigh", "High"),
			LOCTEXT("QualityEpic", "Epic")
		};
	}

	int32 ToQualityIndex(int32 ScalabilityLevel)
	{
		return FMath::Clamp(ScalabilityLevel, 0, MaxQualityLevel);
	}
}

void UGraphicsSettingsViewModel::Initialize()
{
	SetDisplayName(LOCTEXT("GraphicsTabDisplayName", "Graphics"));
	SetTabNameId("GraphicsSettings");

	// Window mode: option order matches EWindowMode (Fullscreen, WindowedFullscreen, Windowed).
	WindowMode = CreateSetting<UOptionSettingViewModel>(LOCTEXT("WindowMode", "Window Mode"));
	WindowMode->Configure(
		{
			LOCTEXT("WindowModeFullscreen", "Fullscreen"),
			LOCTEXT("WindowModeWindowedFullscreen", "Windowed Fullscreen"),
			LOCTEXT("WindowModeWindowed", "Windowed")
		},
		static_cast<int32>(EWindowMode::Windowed));

	// Resolution: supported fullscreen resolutions, plus the current one if it isn't in the list.
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(ResolutionValues);
	if (const UGameUserSettings* CurrentSettings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		ResolutionValues.AddUnique(CurrentSettings->GetScreenResolution());
	}
	ResolutionValues.Sort([](const FIntPoint& A, const FIntPoint& B)
	{
		return A.X != B.X ? A.X < B.X : A.Y < B.Y;
	});

	TArray<FText> ResolutionOptions;
	for (const FIntPoint& Value : ResolutionValues)
	{
		ResolutionOptions.Add(FText::Format(LOCTEXT("ResolutionFormat", "{0} x {1}"),
			FText::AsNumber(Value.X, &FNumberFormattingOptions::DefaultNoGrouping()),
			FText::AsNumber(Value.Y, &FNumberFormattingOptions::DefaultNoGrouping())));
	}
	Resolution = CreateSetting<UOptionSettingViewModel>(LOCTEXT("Resolution", "Resolution"));
	Resolution->Configure(ResolutionOptions, INDEX_NONE);

	// Frame rate limit: 0 means unlimited.
	FrameRateLimitValues = { 30.0f, 60.0f, 120.0f, 144.0f, 0.0f };
	FrameRateLimit = CreateSetting<UOptionSettingViewModel>(LOCTEXT("FrameRateLimit", "Frame Rate Limit"));
	FrameRateLimit->Configure(
		{
			LOCTEXT("FrameRate30", "30 FPS"),
			LOCTEXT("FrameRate60", "60 FPS"),
			LOCTEXT("FrameRate120", "120 FPS"),
			LOCTEXT("FrameRate144", "144 FPS"),
			LOCTEXT("FrameRateUnlimited", "Unlimited")
		},
		FrameRateLimitValues.Num() - 1);

	VSync = CreateSetting<UToggleSettingViewModel>(LOCTEXT("VSync", "VSync"));
	VSync->Configure(false);

	// Quality: the preset first, then the individual settings it controls.
	const TArray<FText> QualityOptions = GraphicsSettings::GetQualityLevelOptions();
	QualityPreset = CreateSetting<UOptionSettingViewModel>(LOCTEXT("QualityPreset", "Quality Preset"));
	QualityPreset->Configure(QualityOptions, GraphicsSettings::MaxQualityLevel, LOCTEXT("QualityCustom", "Custom"));

	ViewDistanceQuality = CreateSetting<UOptionSettingViewModel>(LOCTEXT("ViewDistanceQuality", "View Distance"));
	ShadowQuality = CreateSetting<UOptionSettingViewModel>(LOCTEXT("ShadowQuality", "Shadows"));
	TextureQuality = CreateSetting<UOptionSettingViewModel>(LOCTEXT("TextureQuality", "Textures"));
	EffectsQuality = CreateSetting<UOptionSettingViewModel>(LOCTEXT("EffectsQuality", "Effects"));
	for (UOptionSettingViewModel* QualitySetting : GetIndividualQualitySettings())
	{
		QualitySetting->Configure(QualityOptions, GraphicsSettings::MaxQualityLevel);
	}

	// Dependencies between settings.
	WindowMode->OnValueChanged.AddUObject(this, &ThisClass::HandleWindowModeChanged);
	QualityPreset->OnValueChanged.AddUObject(this, &ThisClass::HandleQualityPresetChanged);
	for (UOptionSettingViewModel* QualitySetting : GetIndividualQualitySettings())
	{
		QualitySetting->OnValueChanged.AddUObject(this, &ThisClass::HandleIndividualQualityChanged);
	}

	HandleWindowModeChanged(WindowMode);
}

void UGraphicsSettingsViewModel::LoadFromSettings(const USettingsGameUserSettings& UserSettings)
{
	WindowMode->SetSelectedIndex(static_cast<int32>(UserSettings.GetFullscreenMode()));
	Resolution->SetSelectedIndex(ResolutionValues.IndexOfByKey(UserSettings.GetScreenResolution()));
	FrameRateLimit->SetSelectedIndex(FindFrameRateLimitIndex(UserSettings.GetFrameRateLimit()));
	VSync->SetValue(UserSettings.IsVSyncEnabled());

	// Set the individual qualities without the preset overwriting them, then derive the preset.
	TGuardValue<bool> Guard(bSyncingQuality, true);
	ViewDistanceQuality->SetSelectedIndex(GraphicsSettings::ToQualityIndex(UserSettings.GetViewDistanceQuality()));
	ShadowQuality->SetSelectedIndex(GraphicsSettings::ToQualityIndex(UserSettings.GetShadowQuality()));
	TextureQuality->SetSelectedIndex(GraphicsSettings::ToQualityIndex(UserSettings.GetTextureQuality()));
	EffectsQuality->SetSelectedIndex(GraphicsSettings::ToQualityIndex(UserSettings.GetVisualEffectQuality()));
	RefreshQualityPreset();
}

void UGraphicsSettingsViewModel::SaveToSettings(USettingsGameUserSettings& UserSettings) const
{
	UserSettings.SetFullscreenMode(EWindowMode::ConvertIntToWindowMode(WindowMode->GetSelectedIndex()));

	if (ResolutionValues.IsValidIndex(Resolution->GetSelectedIndex()))
	{
		UserSettings.SetScreenResolution(ResolutionValues[Resolution->GetSelectedIndex()]);
	}

	if (FrameRateLimitValues.IsValidIndex(FrameRateLimit->GetSelectedIndex()))
	{
		UserSettings.SetFrameRateLimit(FrameRateLimitValues[FrameRateLimit->GetSelectedIndex()]);
	}

	UserSettings.SetVSyncEnabled(VSync->GetValue());

	UserSettings.SetViewDistanceQuality(ViewDistanceQuality->GetSelectedIndex());
	UserSettings.SetShadowQuality(ShadowQuality->GetSelectedIndex());
	UserSettings.SetTextureQuality(TextureQuality->GetSelectedIndex());
	UserSettings.SetVisualEffectQuality(EffectsQuality->GetSelectedIndex());
}

void UGraphicsSettingsViewModel::HandleWindowModeChanged(USettingViewModelBase* Setting)
{
	Resolution->SetIsEditable(WindowMode->GetSelectedIndex() != static_cast<int32>(EWindowMode::WindowedFullscreen));
}

void UGraphicsSettingsViewModel::HandleQualityPresetChanged(USettingViewModelBase* Setting)
{
	const int32 PresetIndex = QualityPreset->GetSelectedIndex();
	if (bSyncingQuality || PresetIndex == INDEX_NONE)
	{
		return;
	}

	TGuardValue<bool> Guard(bSyncingQuality, true);
	for (UOptionSettingViewModel* QualitySetting : GetIndividualQualitySettings())
	{
		QualitySetting->SetSelectedIndex(PresetIndex);
	}
}

void UGraphicsSettingsViewModel::HandleIndividualQualityChanged(USettingViewModelBase* Setting)
{
	if (bSyncingQuality)
	{
		return;
	}

	TGuardValue<bool> Guard(bSyncingQuality, true);
	RefreshQualityPreset();
}

void UGraphicsSettingsViewModel::RefreshQualityPreset()
{
	const TArray<UOptionSettingViewModel*, TInlineAllocator<4>> QualitySettings = GetIndividualQualitySettings();

	int32 SharedIndex = QualitySettings[0]->GetSelectedIndex();
	for (const UOptionSettingViewModel* QualitySetting : QualitySettings)
	{
		if (QualitySetting->GetSelectedIndex() != SharedIndex)
		{
			SharedIndex = INDEX_NONE;
			break;
		}
	}

	QualityPreset->SetSelectedIndex(SharedIndex);
}

int32 UGraphicsSettingsViewModel::FindFrameRateLimitIndex(float Limit) const
{
	const int32 UnlimitedIndex = FrameRateLimitValues.IndexOfByKey(0.0f);
	if (Limit <= 0.0f)
	{
		return UnlimitedIndex;
	}

	int32 ClosestIndex = UnlimitedIndex;
	float ClosestDistance = TNumericLimits<float>::Max();
	for (int32 Index = 0; Index < FrameRateLimitValues.Num(); ++Index)
	{
		const float Distance = FMath::Abs(FrameRateLimitValues[Index] - Limit);
		if (FrameRateLimitValues[Index] > 0.0f && Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestIndex = Index;
		}
	}
	return ClosestIndex;
}

TArray<UOptionSettingViewModel*, TInlineAllocator<4>> UGraphicsSettingsViewModel::GetIndividualQualitySettings() const
{
	return { ViewDistanceQuality, ShadowQuality, TextureQuality, EffectsQuality };
}

#undef LOCTEXT_NAMESPACE
