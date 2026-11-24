// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewModels/GraphicsSettingsViewModel.h"

#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"
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

	const UGameUserSettings* UserSettings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
	
	WindowMode = CreateSetting<UOptionSettingViewModel>(LOCTEXT("WindowMode", "Window Mode"));
	WindowMode->Configure(
		{
			LOCTEXT("WindowModeFullscreen", "Fullscreen"),
			LOCTEXT("WindowModeWindowedFullscreen", "Windowed Fullscreen"),
			LOCTEXT("WindowModeWindowed", "Windowed")
		},
		UserSettings ? static_cast<int32>(UserSettings->GetFullscreenMode()) : static_cast<int32>(EWindowMode::Windowed));

	// Resolution: supported fullscreen resolutions, plus the current one if it isn't in the list.
	const FIntPoint CurrentResolution = UserSettings ? UserSettings->GetScreenResolution() : FIntPoint(1920, 1080);
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(ResolutionValues);
	ResolutionValues.AddUnique(CurrentResolution);
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
	Resolution->Configure(ResolutionOptions, ResolutionValues.IndexOfByKey(CurrentResolution));

	// Frame rate limit: 0 means unlimited. Picks the closest option to the saved value.
	FrameRateLimitValues = { 30.0f, 60.0f, 120.0f, 144.0f, 0.0f };
	const float CurrentLimit = UserSettings ? UserSettings->GetFrameRateLimit() : 0.0f;
	int32 FrameRateIndex = FrameRateLimitValues.Num() - 1;
	if (CurrentLimit > 0.0f)
	{
		float ClosestDistance = TNumericLimits<float>::Max();
		for (int32 Index = 0; Index < FrameRateLimitValues.Num(); ++Index)
		{
			const float Distance = FMath::Abs(FrameRateLimitValues[Index] - CurrentLimit);
			if (FrameRateLimitValues[Index] > 0.0f && Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				FrameRateIndex = Index;
			}
		}
	}
	FrameRateLimit = CreateSetting<UOptionSettingViewModel>(LOCTEXT("FrameRateLimit", "Frame Rate Limit"));
	FrameRateLimit->Configure(
		{
			LOCTEXT("FrameRate30", "30 FPS"),
			LOCTEXT("FrameRate60", "60 FPS"),
			LOCTEXT("FrameRate120", "120 FPS"),
			LOCTEXT("FrameRate144", "144 FPS"),
			LOCTEXT("FrameRateUnlimited", "Unlimited")
		},
		FrameRateIndex);

	VSync = CreateSetting<UToggleSettingViewModel>(LOCTEXT("VSync", "VSync"));
	VSync->Configure(UserSettings ? UserSettings->IsVSyncEnabled() : false);

	// Quality: the preset first, then the individual settings it controls.
	const TArray<FText> QualityOptions = GraphicsSettings::GetQualityLevelOptions();
	QualityPreset = CreateSetting<UOptionSettingViewModel>(LOCTEXT("QualityPreset", "Quality Preset"));

	ViewDistanceQuality = CreateSetting<UOptionSettingViewModel>(LOCTEXT("ViewDistanceQuality", "View Distance"));
	ViewDistanceQuality->Configure(QualityOptions, GraphicsSettings::ToQualityIndex(UserSettings ? UserSettings->GetViewDistanceQuality() : GraphicsSettings::MaxQualityLevel));

	ShadowQuality = CreateSetting<UOptionSettingViewModel>(LOCTEXT("ShadowQuality", "Shadows"));
	ShadowQuality->Configure(QualityOptions, GraphicsSettings::ToQualityIndex(UserSettings ? UserSettings->GetShadowQuality() : GraphicsSettings::MaxQualityLevel));

	TextureQuality = CreateSetting<UOptionSettingViewModel>(LOCTEXT("TextureQuality", "Textures"));
	TextureQuality->Configure(QualityOptions, GraphicsSettings::ToQualityIndex(UserSettings ? UserSettings->GetTextureQuality() : GraphicsSettings::MaxQualityLevel));

	EffectsQuality = CreateSetting<UOptionSettingViewModel>(LOCTEXT("EffectsQuality", "Effects"));
	EffectsQuality->Configure(QualityOptions, GraphicsSettings::ToQualityIndex(UserSettings ? UserSettings->GetVisualEffectQuality() : GraphicsSettings::MaxQualityLevel));

	// Configure the preset from the individual settings so its baseline matches them.
	QualityPreset->Configure(QualityOptions, INDEX_NONE, LOCTEXT("QualityCustom", "Custom"));
	RefreshQualityPreset();
	QualityPreset->CommitBaseline();

	// Dependencies between settings.
	WindowMode->OnValueChanged.AddUObject(this, &ThisClass::HandleWindowModeChanged);
	QualityPreset->OnValueChanged.AddUObject(this, &ThisClass::HandleQualityPresetChanged);
	for (UOptionSettingViewModel* Setting : GetIndividualQualitySettings())
	{
		Setting->OnValueChanged.AddUObject(this, &ThisClass::HandleIndividualQualityChanged);
	}

	HandleWindowModeChanged(WindowMode);
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

TArray<UOptionSettingViewModel*, TInlineAllocator<4>> UGraphicsSettingsViewModel::GetIndividualQualitySettings() const
{
	return { ViewDistanceQuality, ShadowQuality, TextureQuality, EffectsQuality };
}

#undef LOCTEXT_NAMESPACE
