// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewModels/OptionSettingViewModel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(OptionSettingViewModel)

void UOptionSettingViewModel::Configure(const TArray<FText>& InOptions, int32 InitialIndex, const FText& InUnselectedText)
{
	UnselectedText = InUnselectedText;
	UE_MVVM_SET_PROPERTY_VALUE(Options, InOptions);

	SetSelectedIndex(InitialIndex);
	CommitBaseline();

	// The text may have changed even if the index didn't (new options, same index).
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetSelectedOptionText);
}

void UOptionSettingViewModel::SetSelectedIndex(int32 NewIndex)
{
	if (NewIndex != INDEX_NONE && !Options.IsValidIndex(NewIndex))
	{
		return;
	}

	if (UE_MVVM_SET_PROPERTY_VALUE(SelectedIndex, NewIndex))
	{
		// GetSelectedOptionText() is computed from the setting's value, so tell its bindings to re-read it.
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetSelectedOptionText);
		NotifyValueChanged();
	}
}

void UOptionSettingViewModel::SelectNext()
{
	if (IsEditable() && Options.Num() > 0)
	{
		SetSelectedIndex((SelectedIndex + 1) % Options.Num());
	}
}

void UOptionSettingViewModel::SelectPrevious()
{
	if (IsEditable() && Options.Num() > 0)
	{
		SetSelectedIndex(SelectedIndex <= 0 ? Options.Num() - 1 : SelectedIndex - 1);
	}
}

FText UOptionSettingViewModel::GetSelectedOptionText() const
{
	return Options.IsValidIndex(SelectedIndex) ? Options[SelectedIndex] : UnselectedText;
}
