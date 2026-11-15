// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "AudioSettingsViewModel.generated.h"

/**
 * Viewmodel for the Audio tab.
 */
UCLASS(BlueprintType, DisplayName = "Audio Settings Viewmodel")
class MVVM_SETTINGS_API UAudioSettingsViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	float GetMasterVolume() const { return MasterVolume; }
	void SetMasterVolume(float NewMasterVolume);
	
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Audio")
	FText GetMasterVolumeText() const;

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter, Setter, Category = "Audio", meta = (AllowPrivateAccess, UIMin = "0", UIMax = "1", ClampMin = "0", ClampMax = "1"))
	float MasterVolume = 1.0f;
};
