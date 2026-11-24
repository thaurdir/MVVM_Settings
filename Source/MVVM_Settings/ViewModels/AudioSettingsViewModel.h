// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ViewModels/SettingsTabViewModelBase.h"
#include "AudioSettingsViewModel.generated.h"

class USliderSettingViewModel;

// Viewmodel for the Audio tab
UCLASS(BlueprintType, DisplayName = "Audio Settings Viewmodel")
class MVVM_SETTINGS_API UAudioSettingsViewModel : public USettingsTabViewModelBase
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;

	USliderSettingViewModel* GetMasterVolume() const { return MasterVolume; }
	USliderSettingViewModel* GetMusicVolume() const { return MusicVolume; }
	USliderSettingViewModel* GetEffectsVolume() const { return EffectsVolume; }
	USliderSettingViewModel* GetDialogueVolume() const { return DialogueVolume; }

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Audio", meta = (AllowPrivateAccess))
	TObjectPtr<USliderSettingViewModel> MasterVolume;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Audio", meta = (AllowPrivateAccess))
	TObjectPtr<USliderSettingViewModel> MusicVolume;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Audio", meta = (AllowPrivateAccess))
	TObjectPtr<USliderSettingViewModel> EffectsVolume;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Audio", meta = (AllowPrivateAccess))
	TObjectPtr<USliderSettingViewModel> DialogueVolume;
};
