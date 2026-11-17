// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ViewModels/SettingViewModelBase.h"
#include "ToggleSettingViewModel.generated.h"

/** A setting that can be toggled on/off */
UCLASS(BlueprintType, DisplayName = "Toggle Setting Viewmodel")
class MVVM_SETTINGS_API UToggleSettingViewModel : public USettingViewModelBase
{
	GENERATED_BODY()

public:
	void Configure(bool bInitialValue);

	bool GetValue() const { return bValue; }
	void SetValue(bool bNewValue);

	UFUNCTION(BlueprintCallable, Category = "Setting")
	void Toggle();
	
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Setting")
	FText GetValueText() const;

	virtual bool IsDirty() const override { return bValue != bBaselineValue; }
	virtual void RevertToBaseline() override { SetValue(bBaselineValue); }
	virtual void CommitBaseline() override { bBaselineValue = bValue; }

private:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter = "GetValue", Setter = "SetValue", Category = "Setting", meta = (AllowPrivateAccess))
	bool bValue = false;

	bool bBaselineValue = false;
};
