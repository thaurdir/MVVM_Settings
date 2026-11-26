// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SettingsScreenWidget.h"

#include "CommonInputModeTypes.h"
#include "CommonInputSubsystem.h"
#include "Input/UIActionBindingHandle.h"
#include "MVVMSubsystem.h"
#include "UI/ConfirmationDialogWidget.h"
#include "View/MVVMView.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "ViewModels/ControlsSettingsViewModel.h"
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

	// Gamepad-only settings follow the active input method
	if (UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(GetOwningLocalPlayer()))
	{
		InputSubsystem->OnInputMethodChangedNative.AddUObject(this, &ThisClass::HandleInputMethodChanged);
		HandleInputMethodChanged(InputSubsystem->GetCurrentInputType());
	}
}

TOptional<FUIInputConfig> USettingsScreenWidget::GetDesiredInputConfig() const
{
	return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
}

bool USettingsScreenWidget::NativeOnHandleBackAction()
{
	if (SettingsViewModel && SettingsViewModel->HasUnsavedChanges())
	{
		if (DiscardChangesDialogClass)
		{
			if (UConfirmationDialogWidget* Dialog = DialogStack->AddWidget<UConfirmationDialogWidget>(DiscardChangesDialogClass))
			{
				Dialog->OnResult.AddUObject(this, &ThisClass::HandleDiscardChangesResult);
				return true;
			}
		}

		UE_LOG(LogSettingsScreen, Warning, TEXT("%s: no Discard Changes Dialog Class set. Discarding changes without asking."), *GetName());
		SettingsViewModel->RevertChanges();
	}

	return Super::NativeOnHandleBackAction();
}

void USettingsScreenWidget::HandleDiscardChangesResult(bool bConfirmed)
{
	if (bConfirmed)
	{
		SettingsViewModel->RevertChanges();
		DeactivateWidget();
	}
}

void USettingsScreenWidget::HandleInputMethodChanged(ECommonInputType InputType)
{
	SettingsViewModel->GetControls()->SetUsingGamepad(InputType == ECommonInputType::Gamepad);
}
