
#include "EditorMode/AIPointContextEdModeToolkit.h"
#include "Classes/EditorStyleSettings.h"

void FAIPointContextEdModeToolkit::Init(const TSharedPtr< class IToolkitHost >& InitToolkitHost)
{
	const auto& Commands = FAIPointContextEditorCommands::Get();

	GetEditorMode()->GetUICommandList()->MapAction(Commands.AddPoint, FUIAction(FExecuteAction::CreateSP(this, &FAIPointContextEdModeToolkit::OnAddPoint), FCanExecuteAction::CreateSP(this, &FAIPointContextEdModeToolkit::CanAddPoint)));

	AIPointContextEdModeWidget = SNew(SAIPointContextEdModeWidget, SharedThis(this));
	FModeToolkit::Init(InitToolkitHost);
}

FAIPointContextEdMode* FAIPointContextEdModeToolkit::GetEditorMode() const
{
	return (FAIPointContextEdMode*)GLevelEditorModeTools().GetActiveMode(FAIPointContextEdMode::EM_AIPointContext);
}

const TArray<FName> FAIPointContextEdModeToolkit::PaletteNames = { TEXT("ToolMode_Edit") };
void FAIPointContextEdModeToolkit::GetToolPaletteNames(TArray<FName>& InPaletteName) const
{
	if (!GetDefault<UEditorStyleSettings>()->bEnableLegacyEditorModeUI)
	{
		InPaletteName = PaletteNames;
	}
}

void FAIPointContextEdModeToolkit::BuildToolPalette(FName PaletteName, class FToolBarBuilder& ToolbarBuilder)
{
	auto Commands = FAIPointContextEditorCommands::Get();
	FAIPointContextEdMode* EdMode = GetEditorMode();
	
	ToolbarBuilder.BeginSection("Edit");
	//ToolbarBuilder.AddComboButton(Commands.ChangeType, FOnGetContent::CreateSP(this, &FAIPointContextEdModeToolkit::GetPointTypeSelector));
	ToolbarBuilder.AddToolBarButton(Commands.AddPoint);
	ToolbarBuilder.AddSeparator();
	ToolbarBuilder.EndSection();
}

bool FAIPointContextEdModeToolkit::GetIsPropertyVisibleFromProperty(const FProperty& Property) const
{
	return true;
}

void FAIPointContextEdModeToolkit::OnAddPoint()
{
	FAIPointContextEdMode* EdMode = GetEditorMode();
	if (EdMode)
	{
		EdMode->AddPoint(EPointType::Patrol);
	}
}

bool FAIPointContextEdModeToolkit::CanAddPoint() const
{
	return true;
}
