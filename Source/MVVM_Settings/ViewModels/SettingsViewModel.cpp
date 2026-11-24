// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewModels/SettingsViewModel.h"

#include "ViewModels/AccessibilitySettingsViewModel.h"
#include "ViewModels/AudioSettingsViewModel.h"
#include "ViewModels/ControlsSettingsViewModel.h"
#include "ViewModels/GraphicsSettingsViewModel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SettingsViewModel)

void USettingsViewModel::Initialize()
{
	Graphics = NewObject<UGraphicsSettingsViewModel>(this);
	Audio = NewObject<UAudioSettingsViewModel>(this);
	Controls = NewObject<UControlsSettingsViewModel>(this);
	Accessibility = NewObject<UAccessibilitySettingsViewModel>(this);

	for (USettingsTabViewModelBase* Tab : GetTabs())
	{
		Tab->Initialize();
		for (USettingViewModelBase* Setting : Tab->GetSettings())
		{
			Setting->OnValueChanged.AddUObject(this, &ThisClass::HandleSettingValueChanged);
		}
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
		for (const USettingViewModelBase* Setting : Tab->GetSettings())
		{
			if (Setting->IsDirty())
			{
				bAnyDirty = true;
				break;
			}
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
