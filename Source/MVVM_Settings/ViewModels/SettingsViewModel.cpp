// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewModels/SettingsViewModel.h"

#include "Settings/SettingsGameUserSettings.h"
#include "ViewModels/AccessibilitySettingsViewModel.h"
#include "ViewModels/AudioSettingsViewModel.h"
#include "ViewModels/ControlsSettingsViewModel.h"
#include "ViewModels/GraphicsSettingsViewModel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SettingsViewModel)

DEFINE_LOG_CATEGORY_STATIC(LogSettingsViewModel, Log, All);

void USettingsViewModel::Initialize()
{
	Graphics = NewObject<UGraphicsSettingsViewModel>(this);
	Audio = NewObject<UAudioSettingsViewModel>(this);
	Controls = NewObject<UControlsSettingsViewModel>(this);
	Accessibility = NewObject<UAccessibilitySettingsViewModel>(this);

	const USettingsGameUserSettings* UserSettings = USettingsGameUserSettings::Get();
	if (!UserSettings)
	{
		UE_LOG(LogSettingsViewModel, Warning, TEXT("GameUserSettingsClassName isn't set to USettingsGameUserSettings. Settings show defaults and can't be saved."));
	}

	for (USettingsTabViewModelBase* Tab : GetTabs())
	{
		Tab->Initialize();
		if (UserSettings)
		{
			Tab->LoadFromSettings(*UserSettings);
		}
		Tab->CommitBaseline();

		for (USettingViewModelBase* Setting : Tab->GetSettings())
		{
			Setting->OnValueChanged.AddUObject(this, &ThisClass::HandleSettingValueChanged);
		}
	}

	RefreshUnsavedChanges();
}

void USettingsViewModel::ApplyChanges()
{
	USettingsGameUserSettings* UserSettings = USettingsGameUserSettings::Get();
	if (!UserSettings)
	{
		UE_LOG(LogSettingsViewModel, Error, TEXT("Can't apply settings: GameUserSettingsClassName isn't set to USettingsGameUserSettings."));
		return;
	}

	for (const USettingsTabViewModelBase* Tab : GetTabs())
	{
		Tab->SaveToSettings(*UserSettings);
	}

	// Applies resolution and scalability settings, then saves GameUserSettings.ini.
	UserSettings->ApplySettings(false);

	for (USettingsTabViewModelBase* Tab : GetTabs())
	{
		Tab->CommitBaseline();
	}
	RefreshUnsavedChanges();
}

void USettingsViewModel::RevertChanges()
{
	for (USettingsTabViewModelBase* Tab : GetTabs())
	{
		Tab->RevertToBaseline();
	}
	RefreshUnsavedChanges();
}

void USettingsViewModel::ResetToDefaults()
{
	// A temporary settings object holding the defaults, so the tabs can load from it like from saved settings.
	USettingsGameUserSettings* Defaults = NewObject<USettingsGameUserSettings>(GetTransientPackage());
	Defaults->SetToDefaults();

	for (USettingsTabViewModelBase* Tab : GetTabs())
	{
		Tab->LoadFromSettings(*Defaults);
	}
	RefreshUnsavedChanges();
}

void USettingsViewModel::HandleSettingValueChanged(USettingViewModelBase* Setting)
{
	RefreshUnsavedChanges();
}

void USettingsViewModel::RefreshUnsavedChanges()
{
	// Compares against baselines instead of just setting a flag, so changing a setting
	// back to its original value clears the unsaved state.
	bool bAnyDirty = false;
	for (const USettingsTabViewModelBase* Tab : GetTabs())
	{
		if (Tab->IsDirty())
		{
			bAnyDirty = true;
			break;
		}
	}

	UE_MVVM_SET_PROPERTY_VALUE(bHasUnsavedChanges, bAnyDirty);
}

USettingsTabViewModelBase* USettingsViewModel::FindTabByNameId(FName TabNameId) const
{
	for (USettingsTabViewModelBase* Tab : GetTabs())
	{
		if (Tab && Tab->GetTabNameId() == TabNameId)
		{
			return Tab;
		}
	}
	return nullptr;
}

TArray<USettingsTabViewModelBase*, TInlineAllocator<4>> USettingsViewModel::GetTabs() const
{
	return { Graphics, Audio, Controls, Accessibility };
}
