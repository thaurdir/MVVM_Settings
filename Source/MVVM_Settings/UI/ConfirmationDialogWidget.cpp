// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/ConfirmationDialogWidget.h"

#include "CommonInputModeTypes.h"
#include "Input/UIActionBindingHandle.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ConfirmationDialogWidget)

UConfirmationDialogWidget::UConfirmationDialogWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsBackHandler = true;
	bIsModal = true;
}

void UConfirmationDialogWidget::Confirm()
{
	Close(true);
}

void UConfirmationDialogWidget::Cancel()
{
	Close(false);
}

bool UConfirmationDialogWidget::NativeOnHandleBackAction()
{
	Cancel();
	return true;
}

TOptional<FUIInputConfig> UConfirmationDialogWidget::GetDesiredInputConfig() const
{
	return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
}

void UConfirmationDialogWidget::Close(bool bConfirmed)
{
	// Move the listeners out first, so a second click can't report twice.
	FOnConfirmationDialogResult Listeners = MoveTemp(OnResult);
	OnResult.Clear();

	DeactivateWidget();
	Listeners.Broadcast(bConfirmed);
}
