// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "SettingsGameUserSettings.generated.h"

UENUM(BlueprintType)
enum class ESettingsTextSize : uint8
{
	Small,
	Default,
	Large,
	ExtraLarge
};

UENUM(BlueprintType)
enum class ESettingsColorblindMode : uint8
{
	Off,
	Deuteranopia,
	Protanopia,
	Tritanopia
};

/**
 * The project's user settings, saved to GameUserSettings.ini. Graphics use the engine's own
 * fields; the other tabs' settings are added here as Config properties.
 *
 * Set as the engine's GameUserSettingsClassName in DefaultEngine.ini.
 */
UCLASS()
class MVVM_SETTINGS_API USettingsGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	USettingsGameUserSettings(const FObjectInitializer& ObjectInitializer);

	static USettingsGameUserSettings* Get();

	virtual void SetToDefaults() override;

	// Audio (0-1)
	UPROPERTY(Config, BlueprintReadOnly, Category = "Audio")
	float MasterVolume;

	UPROPERTY(Config, BlueprintReadOnly, Category = "Audio")
	float MusicVolume;

	UPROPERTY(Config, BlueprintReadOnly, Category = "Audio")
	float EffectsVolume;

	UPROPERTY(Config, BlueprintReadOnly, Category = "Audio")
	float DialogueVolume;

	// Controls
	UPROPERTY(Config, BlueprintReadOnly, Category = "Controls")
	float LookSensitivity;

	UPROPERTY(Config, BlueprintReadOnly, Category = "Controls")
	bool bInvertY;

	UPROPERTY(Config, BlueprintReadOnly, Category = "Controls")
	bool bVibration;

	// Accessibility
	UPROPERTY(Config, BlueprintReadOnly, Category = "Accessibility")
	ESettingsTextSize TextSize;

	UPROPERTY(Config, BlueprintReadOnly, Category = "Accessibility")
	bool bSubtitles;

	UPROPERTY(Config, BlueprintReadOnly, Category = "Accessibility")
	bool bSubtitleBackground;

	UPROPERTY(Config, BlueprintReadOnly, Category = "Accessibility")
	ESettingsColorblindMode ColorblindMode;

private:
	void ResetProjectSettings();
};
