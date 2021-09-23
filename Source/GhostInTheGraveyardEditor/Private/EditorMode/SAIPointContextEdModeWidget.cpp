
#include "EditorMode/SAIPointContextEdModeWidget.h"
#include "Widgets/Layout/SScrollBox.h"
#include "EditorModeManager.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SSlider.h"
#include "DetailCategoryBuilder.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/STableViewBase.h"
#include "Widgets/Views/STableRow.h"
#include "SlateBasics.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAIPointContextEdModeWidget::Construct(const FArguments& InArgs)
{
	TSharedPtr<SSlider> SphereRadiusSlider;
	TSharedPtr<SSlider> SearchPointSpreadSlider;

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(false, false, false, FDetailsViewArgs::HideNameArea);

	DetailsPanel = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsPanel->SetIsPropertyVisibleDelegate(FIsPropertyVisible::CreateSP(this, &SAIPointContextEdModeWidget::GetIsPropertyVisible));

	FAIPointContextEdMode* EdMode = GetEdMode();
	if (EdMode)
	{
		DetailsPanel->SetObject((UObject*)EdMode->UISettings);
	}

	//TSharedPtr<SNumericEntryBox<float>> SearchPointSpreadBox;

	AddOptions.Add(MakeShareable(new FString("Search Point")));
	AddOptions.Add(MakeShareable(new FString("Sound Transfer Point")));
	AddOptions.Add(MakeShareable(new FString("Patrol Point")));
	//TSharedPtr<SExpanderArrow> Arrow = SNew(SExpanderArrow, FloatTableRow);

	TSharedPtr<STableViewBase> ViewBase;

	ChildSlot
	[
		SNew(SScrollBox)
		+ SScrollBox::Slot().VAlign(VAlign_Top).Padding(5.0f)
		[		
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0F, 5.0F, 0.0F, 0.0F)
			[
 				SNew(STextBlock)
 				.Text(FText::FromString(TEXT("This is a editor mode example.")))
			]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0F, 5.0F, 0.0F, 0.0F)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().AutoWidth().Padding(2, 0, 0, 0).VAlign(VAlign_Center)
				[
					SNew(SButton)
					.Text(FText::FromString("Add"))
					.OnClicked(this, &SAIPointContextEdModeWidget::OnAddPoint)
					.IsEnabled(this, &SAIPointContextEdModeWidget::CanAddPoint)
				]
				+ SHorizontalBox::Slot().AutoWidth().Padding(0, 0, 2, 0).VAlign(VAlign_Center)
				[
					SNew(SButton).Text(FText::FromString("Remove")).OnClicked(this, &SAIPointContextEdModeWidget::OnRemovePoint).IsEnabled(this, &SAIPointContextEdModeWidget::CanRemovePoint)
				]
			]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0F, 5.0F, 0.0F, 0.0F)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(STextBlock).Text(FText::FromString("Sphere Radius"))
				]
				+ SHorizontalBox::Slot()
				[
					SNew(SNumericEntryBox<float>).AllowSpin(true).MinValue(1.0F)
					.OnValueChanged(this, &SAIPointContextEdModeWidget::OnChangeDebugSphereRadius)
					.Value(this, &SAIPointContextEdModeWidget::GetDebugSphereRadius)
				]
			]
			+ SVerticalBox::Slot().AutoHeight()
			[
				SNew(SComboBox<TSharedPtr<FString>>).OptionsSource(&AddOptions)
				.OnSelectionChanged(this, &SAIPointContextEdModeWidget::OnSelectionChanged)
				.OnGenerateWidget(this, &SAIPointContextEdModeWidget::MakeWidgetForOption)
				.InitiallySelectedItem(AddOptions[CurrentPointType])
				[
					SNew(STextBlock)
					.Text(this, &SAIPointContextEdModeWidget::GetCurrentPointTypeLabel)
				]
			]
			+ SVerticalBox::Slot().AutoHeight()
			[
				SNew(SButton).Text(FText::FromString("Generate Search Points"))
				.OnClicked(this, &SAIPointContextEdModeWidget::OnGenerateSearchPoints)
			]
			+ SVerticalBox::Slot().Padding(0)
			[
				DetailsPanel.ToSharedRef()
			]
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

bool SAIPointContextEdModeWidget::GetIsPropertyVisible(const FPropertyAndParent& PropertyAndParent) const
{
	return true;
}

FAIPointContextEdMode* SAIPointContextEdModeWidget::GetEdMode() const
{
	return (FAIPointContextEdMode*)GLevelEditorModeTools().GetActiveMode(FAIPointContextEdMode::EM_AIPointContext);
}

TSharedRef<SWidget> SAIPointContextEdModeWidget::MakeWidgetForOption(TSharedPtr<FString> InOption)
{
	return SNew(STextBlock).Text(FText::FromString(*InOption));
}

FReply SAIPointContextEdModeWidget::OnAddPoint()
{
	GetEdMode()->AddPoint(EPointType::Search);
	return FReply::Handled();
}

bool SAIPointContextEdModeWidget::CanAddPoint() const
{
	return GetEdMode()->CanAddPoint(EPointType::Search);
}

FReply SAIPointContextEdModeWidget::OnRemovePoint()
{
	GetEdMode()->RemovePoint();
	return FReply::Handled();
}

bool SAIPointContextEdModeWidget::CanRemovePoint() const
{
	return GetEdMode()->CanRemovePoint();
}

void SAIPointContextEdModeWidget::OnChangeDebugSphereRadius(float Value)
{
	GetEdMode()->SetDebugSphereRadius(Value);
}

FText SAIPointContextEdModeWidget::GetCurrentPointTypeLabel() const
{
	if(AddOptions.IsValidIndex(CurrentPointType))
		return FText::FromString(*AddOptions[CurrentPointType]);

	return FText::FromString("Invalid Point Type");
}

void SAIPointContextEdModeWidget::OnSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type)
{
	int32 Index = -1;

	for (int32 i = 0; i < AddOptions.Num(); ++i)
	{
		if (AddOptions[i] == NewValue)
		{
			Index = i;
			break;
		}
	}

	if(Index != -1)
		CurrentPointType = Index;
}

FReply SAIPointContextEdModeWidget::OnGenerateSearchPoints()
{
	//GetEdMode()->GenerateSearchPoints();
	return FReply::Handled();
}

void SAIPointContextEdModeWidget::ClearSearchPoints()
{
	GetEdMode()->ClearSearchPoints();
}

void SAIPointContextEdModeWidget::OnChangeSearchPointSpread(float Value)
{
	//GetEdMode()->SetSearchPointGenSpread(Value);
}

void SAIPointContextEdModeWidget::OnChangeSearchPointExtent(float Value)
{
	//GetEdMode()->SetSearchPointGenExtent(Value);
}

