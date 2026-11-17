// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "SettingViewModelBase.generated.h"

class USettingViewModelBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSettingValueChanged, USettingViewModelBase* /*Setting*/);

/**
 * Base class for a single setting shown in the menu.
 *
 * Each setting keeps a "baseline" (the last applied value) so it can report whether it has
 * unsaved changes and revert to it. Native code listens to OnValueChanged; widgets bind to the
 * FieldNotify properties instead.
 */
UCLASS(Abstract, BlueprintType)
class MVVM_SETTINGS_API USettingViewModelBase : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	const FText& GetDisplayName() const { return DisplayName; }
	void SetDisplayName(const FText& NewDisplayName);

	bool IsEditable() const { return bIsEditable; }
	void SetIsEditable(bool bNewIsEditable);

	/** True when the current value differs from the baseline. */
	virtual bool IsDirty() const PURE_VIRTUAL(USettingViewModelBase::IsDirty, return false;);
	
	virtual void RevertToBaseline() PURE_VIRTUAL(USettingViewModelBase::RevertToBaseline, );
	virtual void CommitBaseline() PURE_VIRTUAL(USettingViewModelBase::CommitBaseline, );

	/** Broadcast whenever the setting's value changes, from the UI or from code. */
	FOnSettingValueChanged OnValueChanged;

protected:
	/** Subclasses call this after their value actually changed. */
	void NotifyValueChanged() { OnValueChanged.Broadcast(this); }

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Setting", meta = (AllowPrivateAccess))
	FText DisplayName;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter = "IsEditable", Category = "Setting", meta = (AllowPrivateAccess))
	bool bIsEditable = true;
};
