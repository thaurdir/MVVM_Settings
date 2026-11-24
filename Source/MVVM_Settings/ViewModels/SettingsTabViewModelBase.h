// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "ViewModels/SettingViewModelBase.h"
#include "SettingsTabViewModelBase.generated.h"

/**
 * Base class for one tab of the settings menu. A tab owns its setting (row) viewmodels.
 *
 * Subclasses expose each setting as its own FieldNotify property so a tab widget can bind
 * row widgets to them by name. All settings are also kept in the Settings array, which code
 * uses to check or revert the whole tab, and which a ListView could display directly.
 */
UCLASS(Abstract, BlueprintType)
class MVVM_SETTINGS_API USettingsTabViewModelBase : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	// Creates the tab's settings and loads their current values
	virtual void Initialize() PURE_VIRTUAL(USettingsTabViewModelBase::Initialize, );
	
	const FText& GetDisplayName() const { return DisplayName; }
	const FName& GetTabNameId() const { return TabNameId; }

	const TArray<TObjectPtr<USettingViewModelBase>>& GetSettings() const { return Settings; }

protected:
	// Creates a setting owned by this tab and registers it in Settings
	template <typename TSetting>
	TSetting* CreateSetting(const FText& DisplayName)
	{
		TSetting* Setting = NewObject<TSetting>(this);
		Setting->SetDisplayName(DisplayName);
		Settings.Add(Setting);
		return Setting;
	}
	
	void SetDisplayName(const FText& NewDisplayName)
	{
		UE_MVVM_SET_PROPERTY_VALUE(DisplayName, NewDisplayName);
	}
	
	void SetTabNameId(const FName& NewTabNameId)
	{
		UE_MVVM_SET_PROPERTY_VALUE(TabNameId, NewTabNameId);
	}

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Setting", meta = (AllowPrivateAccess))
	FText DisplayName;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Setting", meta = (AllowPrivateAccess))
	FName TabNameId;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Settings", meta = (AllowPrivateAccess))
	TArray<TObjectPtr<USettingViewModelBase>> Settings;
};
