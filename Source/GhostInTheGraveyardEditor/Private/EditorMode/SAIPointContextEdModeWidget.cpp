
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
#include "EditorMode/AIPointContextEdModeToolkit.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widgets/SOverlay.h"
#include "Classes/EditorStyleSettings.h"
#include "SlateBasics.h"

#define LOCTEXT_NAMESPACE "AIPointContextEditor"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAIPointContextEdModeWidget::Construct(const FArguments& InArgs, TSharedRef<FAIPointContextEdModeToolkit> InParentToolkit)
{
	ParentToolkit = &InParentToolkit.Get();
	TSharedPtr<SSlider> SphereRadiusSlider;
	TSharedPtr<SSlider> SearchPointSpreadSlider;

	TSharedRef<FUICommandList> CommandList = InParentToolkit->GetToolkitCommands();

	FToolBarBuilder ModeSwitchButtons(CommandList, FMultiBoxCustomization::None);
	{
//		ModeSwitchButtons.AddToolBarButton(FAIPointContextEditorCommands::Get().AddPoint, NAME_None, LOCTEXT("Add Point", "Add"), LOCTEXT("AddPoint.Tooltip", "Adds a point"));
// 		ModeSwitchButtons.AddToolBarButton(FLandscapeEditorCommands::Get().SculptMode, NAME_None, LOCTEXT("Mode.Sculpt", "Sculpt"), LOCTEXT("Mode.Sculpt.Tooltip", "Contains tools that modify the shape of a landscape"));
// 		ModeSwitchButtons.AddToolBarButton(FLandscapeEditorCommands::Get().PaintMode, NAME_None, LOCTEXT("Mode.Paint", "Paint"), LOCTEXT("Mode.Paint.Tooltip", "Contains tools that paint materials on to a landscape"));
	}

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(false, false, false, FDetailsViewArgs::HideNameArea);

	DetailsPanel = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	//DetailsPanel->SetIsPropertyVisibleDelegate(FIsPropertyVisible::CreateSP(this, &SAIPointContextEdModeWidget::GetIsPropertyVisible));

	FAIPointContextEdMode* EdMode = GetEdMode();
	if (EdMode)
	{
		DetailsPanel->SetObject((UObject*)EdMode->UISettings);
	}

	//AddOptions.Add(MakeShareable(new FString("Search Point")));
	//AddOptions.Add(MakeShareable(new FString("Sound Transfer Point")));
	//AddOptions.Add(MakeShareable(new FString("Patrol Point")));


	ChildSlot
	[
// 		SNew(SVerticalBox)
// 		+ SVerticalBox::Slot()
// 		.AutoHeight()
// 		.Padding(0, 0, 0, 5)
// 		[
// 			SAssignNew(Error, SErrorText)
// 		]
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(0)
		[
			SNew(SVerticalBox)
			.IsEnabled(this, &SAIPointContextEdModeWidget::GetAIPointContextEditorEnabled)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(4, 0, 4, 5)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				[
	 				SNew(SBorder)
					.Visibility_Lambda([]() -> EVisibility { return GetDefault<UEditorStyleSettings>()->bEnableLegacyEditorModeUI ? EVisibility::Visible : EVisibility::Collapsed; })
					.HAlign(HAlign_Center)
					[
					 	ModeSwitchButtons.MakeWidget()
					]
				]
			]
			+ SVerticalBox::Slot()
			.Padding(0)
			[
				DetailsPanel.ToSharedRef()
			]
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

bool SAIPointContextEdModeWidget::GetIsPropertyVisible(const FPropertyAndParent& PropertyAndParent) const
{
	return ParentToolkit->GetIsPropertyVisibleFromProperty(PropertyAndParent.Property);
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
	//GetEdMode()->AddPoint(EPointType::Search);
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

bool SAIPointContextEdModeWidget::GetAIPointContextEditorEnabled() const
{
	FAIPointContextEdMode* EdMode = GetEdMode();
	if (EdMode)
	{
		return true;
	}

	return false;
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

#undef LOCTEXT_NAMESPACE