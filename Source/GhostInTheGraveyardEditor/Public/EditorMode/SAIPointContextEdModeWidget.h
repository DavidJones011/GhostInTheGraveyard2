
#pragma once

#include "Framework/Application/SlateApplication.h"
#include "EditorMode/AIPointContextEdMode.h"

class SAIPointContextEdModeWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAIPointContextEdModeWidget) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	bool GetIsPropertyVisible(const FPropertyAndParent& PropertyAndParent) const;
	class FAIPointContextEdMode* GetEdMode() const;

	TSharedRef<SWidget> MakeWidgetForOption(TSharedPtr<FString> InOption);

	FReply OnAddPoint();
	bool CanAddPoint() const;
	FReply OnRemovePoint();
	bool CanRemovePoint() const;
	void OnChangeDebugSphereRadius(float Value);
	FText GetCurrentPointTypeLabel() const;
	void OnSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type);

	void OnChangeSearchPointSpread(float Value);
	void OnChangeSearchPointExtent(float Value);
	
	void ClearSearchPoints();
	FReply OnGenerateSearchPoints();

	TArray<TSharedPtr<FString>> AddOptions;
	TSharedPtr<class IDetailsView> DetailsPanel;
	uint32 CurrentPointType = 0;

	// Getters for Details Panel
	//TOptional<float> GetSearchPointGenSpread() const { return GetEdMode()->GetSearchPointGenSpread(); }
	//TOptional<float> GetSearchPointGenExtent() const { return GetEdMode()->GetSearchPointGenExtent(); }
	TOptional<float> GetDebugSphereRadius()  const { return GetEdMode()->GetDebugSphereRadius(); }
};