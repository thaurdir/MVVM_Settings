// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ViewModels/SettingViewModelBase.h"
#include "OptionSettingViewModel.generated.h"

/** A setting that picks one option from a list */
UCLASS(BlueprintType, DisplayName = "Option Setting Viewmodel")
class MVVM_SETTINGS_API UOptionSettingViewModel : public USettingViewModelBase
{
	GENERATED_BODY()

public:
	void Configure(const TArray<FText>& InOptions, int32 InitialIndex, const FText& InUnselectedText = FText::GetEmpty());

	const TArray<FText>& GetOptions() const { return Options; }

	int32 GetSelectedIndex() const { return SelectedIndex; }
	
	void SetSelectedIndex(int32 NewIndex);

	UFUNCTION(BlueprintCallable, Category = "Setting")
	void SelectNext();

	UFUNCTION(BlueprintCallable, Category = "Setting")
	void SelectPrevious();

	UFUNCTION(BlueprintPure, FieldNotify, Category = "Setting")
	FText GetSelectedOptionText() const;

	virtual bool IsDirty() const override { return SelectedIndex != BaselineIndex; }
	virtual void RevertToBaseline() override { SetSelectedIndex(BaselineIndex); }
	virtual void CommitBaseline() override { BaselineIndex = SelectedIndex; }

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Setting", meta = (AllowPrivateAccess))
	TArray<FText> Options;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter, Setter, Category = "Setting", meta = (AllowPrivateAccess))
	int32 SelectedIndex = INDEX_NONE;

	FText UnselectedText;
	int32 BaselineIndex = INDEX_NONE;
};
