// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ViewModels/SettingsTabViewModelBase.h"
#include "ControlsSettingsViewModel.generated.h"

class USliderSettingViewModel;
class UToggleSettingViewModel;

// Viewmodel for the Controls tab
UCLASS(BlueprintType, DisplayName = "Controls Settings Viewmodel")
class MVVM_SETTINGS_API UControlsSettingsViewModel : public USettingsTabViewModelBase
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;
	virtual void LoadFromSettings(const USettingsGameUserSettings& UserSettings) override;
	virtual void SaveToSettings(USettingsGameUserSettings& UserSettings) const override;

	USliderSettingViewModel* GetLookSensitivity() const { return LookSensitivity; }
	UToggleSettingViewModel* GetInvertY() const { return InvertY; }
	UToggleSettingViewModel* GetVibration() const { return Vibration; }

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Controls", meta = (AllowPrivateAccess))
	TObjectPtr<USliderSettingViewModel> LookSensitivity;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Controls", meta = (AllowPrivateAccess))
	TObjectPtr<UToggleSettingViewModel> InvertY;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Controls", meta = (AllowPrivateAccess))
	TObjectPtr<UToggleSettingViewModel> Vibration;
};
