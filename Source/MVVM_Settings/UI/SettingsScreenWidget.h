// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SettingsScreenWidget.generated.h"

class UCommonActivatableWidgetStack;
class UConfirmationDialogWidget;
class USettingsViewModel;

// Base class for the settings screen (WBP_SettingsScreen)
UCLASS(Abstract, Blueprintable)
class MVVM_SETTINGS_API USettingsScreenWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	USettingsScreenWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "Settings")
	USettingsViewModel* GetSettingsViewModel() const { return SettingsViewModel; }

protected:
	virtual void NativeOnInitialized() override;
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

	// With unsaved changes, asks before closing. Otherwise closes the screen
	virtual bool NativeOnHandleBackAction() override;

	// Shown when backing out with unsaved changes
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TSubclassOf<UConfirmationDialogWidget> DiscardChangesDialogClass;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> DialogStack;

private:
	void HandleDiscardChangesResult(bool bConfirmed);

	UPROPERTY(Transient)
	TObjectPtr<USettingsViewModel> SettingsViewModel;
};
