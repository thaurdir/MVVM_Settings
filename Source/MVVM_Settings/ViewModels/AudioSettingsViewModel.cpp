// Fill out your copyright notice in the Description page of Project Settings.

#include "AudioSettingsViewModel.h"

#include "HAL/IConsoleManager.h"
#include "UObject/UObjectIterator.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AudioSettingsViewModel)

void UAudioSettingsViewModel::SetMasterVolume(float NewMasterVolume)
{
	// Compares, assigns, and broadcasts the MasterVolume's change event only if the value is changed.
	if (UE_MVVM_SET_PROPERTY_VALUE(MasterVolume, FMath::Clamp(NewMasterVolume, 0.0f, 1.0f)))
	{
		// GetMasterVolumeText() is computed from MasterVolume, so tell its bindings to re-read it.
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMasterVolumeText);
	}
}

FText UAudioSettingsViewModel::GetMasterVolumeText() const
{
	return FText::AsPercent(MasterVolume);
}

#if !UE_BUILD_SHIPPING
// Debug-only console command for testing the C++ -> UI direction
static FAutoConsoleCommand GSetMasterVolumeCommand(
	TEXT("Settings.Audio.SetMasterVolume"),
	TEXT("Sets MasterVolume (0-1) on every live UAudioSettingsViewModel. Usage: Settings.Audio.SetMasterVolume <value>"),
	FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
	{
		if (Args.Num() < 1)
		{
			return;
		}

		const float NewValue = FCString::Atof(*Args[0]);
		for (TObjectIterator<UAudioSettingsViewModel> It; It; ++It)
		{
			// Skip the class default object; only live instances are bound to widgets.
			if (!It->HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
			{
				It->SetMasterVolume(NewValue);
			}
		}
	}));
#endif
