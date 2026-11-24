// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "SettingsViewModel.generated.h"

class UAccessibilitySettingsViewModel;
class UAudioSettingsViewModel;
class UControlsSettingsViewModel;
class UGraphicsSettingsViewModel;
class USettingViewModelBase;
class USettingsTabViewModelBase;

/**
 * Root viewmodel for the settings screen. Owns one viewmodel per tab and tracks whether
 * any setting differs from its last applied value.
 */
UCLASS(BlueprintType, DisplayName = "Settings Viewmodel")
class MVVM_SETTINGS_API USettingsViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	// Creates and initializes the tab viewmodels. Call once, before handing this to a view
	void Initialize();

	UGraphicsSettingsViewModel* GetGraphics() const { return Graphics; }
	UAudioSettingsViewModel* GetAudio() const { return Audio; }
	UControlsSettingsViewModel* GetControls() const { return Controls; }
	UAccessibilitySettingsViewModel* GetAccessibility() const { return Accessibility; }

	bool HasUnsavedChanges() const { return bHasUnsavedChanges; }
	
	UFUNCTION(BlueprintPure, Category = "Settings")
	USettingsTabViewModelBase* FindTabByNameId(FName TabNameId) const;

private:
	void HandleSettingValueChanged(USettingViewModelBase* Setting);
	void RefreshUnsavedChanges();

	TArray<USettingsTabViewModelBase*, TInlineAllocator<4>> GetTabs() const;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Settings", meta = (AllowPrivateAccess))
	TObjectPtr<UGraphicsSettingsViewModel> Graphics;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Settings", meta = (AllowPrivateAccess))
	TObjectPtr<UAudioSettingsViewModel> Audio;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Settings", meta = (AllowPrivateAccess))
	TObjectPtr<UControlsSettingsViewModel> Controls;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Settings", meta = (AllowPrivateAccess))
	TObjectPtr<UAccessibilitySettingsViewModel> Accessibility;

	// True while any setting differs from its baseline
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter = "HasUnsavedChanges", Category = "Settings", meta = (AllowPrivateAccess))
	bool bHasUnsavedChanges = false;
};
