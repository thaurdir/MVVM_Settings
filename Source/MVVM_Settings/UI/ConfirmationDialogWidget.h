// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ConfirmationDialogWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnConfirmationDialogResult, bool /*bConfirmed*/);

// Base class for a modal yes/no dialog. Back counts as Cancel
UCLASS(Abstract, Blueprintable)
class MVVM_SETTINGS_API UConfirmationDialogWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UConfirmationDialogWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void Confirm();

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void Cancel();

	// Broadcast once when the dialog closes. Cleared afterwards, since stacks reuse dialog instances
	FOnConfirmationDialogResult OnResult;

protected:
	virtual bool NativeOnHandleBackAction() override;
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

private:
	void Close(bool bConfirmed);
};
