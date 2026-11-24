// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ViewModels/SettingsTabViewModelBase.h"
#include "GraphicsSettingsViewModel.generated.h"

class UOptionSettingViewModel;
class UToggleSettingViewModel;

// Viewmodel for the Graphics tab
UCLASS(BlueprintType, DisplayName = "Graphics Settings Viewmodel")
class MVVM_SETTINGS_API UGraphicsSettingsViewModel : public USettingsTabViewModelBase
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;
	virtual void LoadFromSettings(const USettingsGameUserSettings& UserSettings) override;
	virtual void SaveToSettings(USettingsGameUserSettings& UserSettings) const override;

	UOptionSettingViewModel* GetWindowMode() const { return WindowMode; }
	UOptionSettingViewModel* GetResolution() const { return Resolution; }
	UOptionSettingViewModel* GetFrameRateLimit() const { return FrameRateLimit; }
	UToggleSettingViewModel* GetVSync() const { return VSync; }
	UOptionSettingViewModel* GetQualityPreset() const { return QualityPreset; }
	UOptionSettingViewModel* GetViewDistanceQuality() const { return ViewDistanceQuality; }
	UOptionSettingViewModel* GetShadowQuality() const { return ShadowQuality; }
	UOptionSettingViewModel* GetTextureQuality() const { return TextureQuality; }
	UOptionSettingViewModel* GetEffectsQuality() const { return EffectsQuality; }

private:
	void HandleWindowModeChanged(USettingViewModelBase* Setting);
	void HandleQualityPresetChanged(USettingViewModelBase* Setting);
	void HandleIndividualQualityChanged(USettingViewModelBase* Setting);
	
	void RefreshQualityPreset();

	// Index of the FrameRateLimitValues entry closest to Limit (0 = unlimited)
	int32 FindFrameRateLimitIndex(float Limit) const;

	TArray<UOptionSettingViewModel*, TInlineAllocator<4>> GetIndividualQualitySettings() const;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Graphics", meta = (AllowPrivateAccess))
	TObjectPtr<UOptionSettingViewModel> WindowMode;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Graphics", meta = (AllowPrivateAccess))
	TObjectPtr<UOptionSettingViewModel> Resolution;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Graphics", meta = (AllowPrivateAccess))
	TObjectPtr<UOptionSettingViewModel> FrameRateLimit;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Graphics", meta = (AllowPrivateAccess))
	TObjectPtr<UToggleSettingViewModel> VSync;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Graphics", meta = (AllowPrivateAccess))
	TObjectPtr<UOptionSettingViewModel> QualityPreset;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Graphics", meta = (AllowPrivateAccess))
	TObjectPtr<UOptionSettingViewModel> ViewDistanceQuality;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Graphics", meta = (AllowPrivateAccess))
	TObjectPtr<UOptionSettingViewModel> ShadowQuality;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Graphics", meta = (AllowPrivateAccess))
	TObjectPtr<UOptionSettingViewModel> TextureQuality;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Graphics", meta = (AllowPrivateAccess))
	TObjectPtr<UOptionSettingViewModel> EffectsQuality;

	// The values listed as Resolution and FrameRateLimit options
	TArray<FIntPoint> ResolutionValues;
	TArray<float> FrameRateLimitValues;

	// Stops the preset and individual settings from updating each other in a loop
	bool bSyncingQuality = false;
};
