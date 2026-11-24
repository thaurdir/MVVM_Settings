// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ViewModels/SettingsTabViewModelBase.h"
#include "AccessibilitySettingsViewModel.generated.h"

class UOptionSettingViewModel;
class UToggleSettingViewModel;

// Viewmodel for the Accessibility tab.
UCLASS(BlueprintType, DisplayName = "Accessibility Settings Viewmodel")
class MVVM_SETTINGS_API UAccessibilitySettingsViewModel : public USettingsTabViewModelBase
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;

	UOptionSettingViewModel* GetTextSize() const { return TextSize; }
	UToggleSettingViewModel* GetSubtitles() const { return Subtitles; }
	UToggleSettingViewModel* GetSubtitleBackground() const { return SubtitleBackground; }
	UOptionSettingViewModel* GetColorblindMode() const { return ColorblindMode; }

private:
	void HandleSubtitlesChanged(USettingViewModelBase* Setting);

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Accessibility", meta = (AllowPrivateAccess))
	TObjectPtr<UOptionSettingViewModel> TextSize;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Accessibility", meta = (AllowPrivateAccess))
	TObjectPtr<UToggleSettingViewModel> Subtitles;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Accessibility", meta = (AllowPrivateAccess))
	TObjectPtr<UToggleSettingViewModel> SubtitleBackground;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Accessibility", meta = (AllowPrivateAccess))
	TObjectPtr<UOptionSettingViewModel> ColorblindMode;
};
