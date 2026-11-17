// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewModels/SliderSettingViewModel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SliderSettingViewModel)

void USliderSettingViewModel::Configure(float InMinValue, float InMaxValue, float InStepSize, float InitialValue, ESliderValueFormat InFormat)
{
	Format = InFormat;
	UE_MVVM_SET_PROPERTY_VALUE(MinValue, InMinValue);
	UE_MVVM_SET_PROPERTY_VALUE(MaxValue, InMaxValue);
	UE_MVVM_SET_PROPERTY_VALUE(StepSize, InStepSize);

	SetValue(InitialValue);
	CommitBaseline();
}

void USliderSettingViewModel::SetValue(float NewValue)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Value, FMath::Clamp(NewValue, MinValue, MaxValue)))
	{
		// GetValueText() is computed from the setting's value, so tell its bindings to re-read it.
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetValueText);
		NotifyValueChanged();
	}
}

FText USliderSettingViewModel::GetValueText() const
{
	if (Format == ESliderValueFormat::Percent)
	{
		return FText::AsPercent(Value);
	}

	FNumberFormattingOptions Options;
	Options.MinimumFractionalDigits = 1;
	Options.MaximumFractionalDigits = 2;
	return FText::AsNumber(Value, &Options);
}

bool USliderSettingViewModel::IsDirty() const
{
	return !FMath::IsNearlyEqual(Value, BaselineValue, KINDA_SMALL_NUMBER);
}

void USliderSettingViewModel::RevertToBaseline()
{
	SetValue(BaselineValue);
}

void USliderSettingViewModel::CommitBaseline()
{
	BaselineValue = Value;
}
