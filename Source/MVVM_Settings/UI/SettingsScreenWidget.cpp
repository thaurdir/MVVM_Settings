// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SettingsScreenWidget.h"

#include "CommonInputModeTypes.h"
#include "Input/UIActionBindingHandle.h"
#include "MVVMSubsystem.h"
#include "View/MVVMView.h"
#include "ViewModels/SettingsViewModel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SettingsScreenWidget)

DEFINE_LOG_CATEGORY_STATIC(LogSettingsScreen, Log, All);

USettingsScreenWidget::USettingsScreenWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Back (Escape / B) closes the screen
	bIsBackHandler = true;
}

void USettingsScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SettingsViewModel = NewObject<USettingsViewModel>(this);
	SettingsViewModel->Initialize();

	// Hand the viewmodel to the MVVM view. This fills the Blueprint's "Manual" viewmodel slot of the matching class
	UMVVMView* View = UMVVMSubsystem::GetViewFromUserWidget(this);
	if (!View || !View->SetViewModelByClass(SettingsViewModel))
	{
		UE_LOG(LogSettingsScreen, Warning,
			TEXT("%s: couldn't set the Settings Viewmodel. Add one in the widget's Viewmodels panel with Creation Type 'Manual'."),
			*GetName());
	}
}

TOptional<FUIInputConfig> USettingsScreenWidget::GetDesiredInputConfig() const
{
	return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
}
