// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewModels/SettingsTabViewModelBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SettingsTabViewModelBase)

bool USettingsTabViewModelBase::IsDirty() const
{
	return Settings.ContainsByPredicate([](const USettingViewModelBase* Setting)
	{
		return Setting->IsDirty();
	});
}

void USettingsTabViewModelBase::RevertToBaseline()
{
	for (USettingViewModelBase* Setting : Settings)
	{
		Setting->RevertToBaseline();
	}
}

void USettingsTabViewModelBase::CommitBaseline()
{
	for (USettingViewModelBase* Setting : Settings)
	{
		Setting->CommitBaseline();
	}
}
