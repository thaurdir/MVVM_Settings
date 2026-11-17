// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewModels/ToggleSettingViewModel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ToggleSettingViewModel)

#define LOCTEXT_NAMESPACE "Settings"

void UToggleSettingViewModel::Configure(bool bInitialValue)
{
	SetValue(bInitialValue);
	CommitBaseline();
}

void UToggleSettingViewModel::SetValue(bool bNewValue)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(bValue, bNewValue))
	{
		// GetValueText() is computed from the setting's value, so tell its bindings to re-read it.
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetValueText);
		NotifyValueChanged();
	}
}

void UToggleSettingViewModel::Toggle()
{
	if (IsEditable())
	{
		SetValue(!bValue);
	}
}

FText UToggleSettingViewModel::GetValueText() const
{
	return bValue ? LOCTEXT("ToggleOn", "On") : LOCTEXT("ToggleOff", "Off");
}

#undef LOCTEXT_NAMESPACE
