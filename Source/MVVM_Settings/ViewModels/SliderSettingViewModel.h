// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ViewModels/SettingViewModelBase.h"
#include "SliderSettingViewModel.generated.h"

UENUM(BlueprintType)
enum class ESliderValueFormat : uint8
{
	Percent,
	Decimal
};

/** A float setting shown as a slider */
UCLASS(BlueprintType, DisplayName = "Slider Setting Viewmodel")
class MVVM_SETTINGS_API USliderSettingViewModel : public USettingViewModelBase
{
	GENERATED_BODY()

public:
	void Configure(float InMinValue, float InMaxValue, float InStepSize, float InitialValue, ESliderValueFormat InFormat);

	float GetValue() const { return Value; }
	void SetValue(float NewValue);

	float GetMinValue() const { return MinValue; }
	float GetMaxValue() const { return MaxValue; }
	float GetStepSize() const { return StepSize; }
	
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Setting")
	FText GetValueText() const;

	virtual bool IsDirty() const override;
	virtual void RevertToBaseline() override;
	virtual void CommitBaseline() override;

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter, Setter, Category = "Setting", meta = (AllowPrivateAccess))
	float Value = 0.0f;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Setting", meta = (AllowPrivateAccess))
	float MinValue = 0.0f;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Setting", meta = (AllowPrivateAccess))
	float MaxValue = 1.0f;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Setting", meta = (AllowPrivateAccess))
	float StepSize = 0.01f;

	ESliderValueFormat Format = ESliderValueFormat::Percent;
	float BaselineValue = 0.0f;
};
