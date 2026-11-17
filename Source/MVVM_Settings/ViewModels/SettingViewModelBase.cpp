// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewModels/SettingViewModelBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SettingViewModelBase)

void USettingViewModelBase::SetDisplayName(const FText& NewDisplayName)
{
	UE_MVVM_SET_PROPERTY_VALUE(DisplayName, NewDisplayName);
}

void USettingViewModelBase::SetIsEditable(bool bNewIsEditable)
{
	UE_MVVM_SET_PROPERTY_VALUE(bIsEditable, bNewIsEditable);
}
