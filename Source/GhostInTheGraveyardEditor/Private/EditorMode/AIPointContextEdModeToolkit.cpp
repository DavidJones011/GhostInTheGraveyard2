
#include "EditorMode/AIPointContextEdModeToolkit.h"
#include "Classes/EditorStyleSettings.h"

void FAIPointContextEdModeToolkit::Init(const TSharedPtr< class IToolkitHost >& InitToolkitHost)
{
	MapCommands();

	AIPointContextEdModeWidget = SNew(SAIPointContextEdModeWidget, SharedThis(this));
	FModeToolkit::Init(InitToolkitHost);
}

void FAIPointContextEdModeToolkit::MapCommands()
{
	const auto& Commands = FAIPointContextEditorCommands::Get();
	TSharedRef<FUICommandList> CommandList = GetEditorMode()->GetUICommandList();

	CommandList->MapAction(Commands.AddPoint, 
		FUIAction(FExecuteAction::CreateSP(this, &FAIPointContextEdModeToolkit::OnAddPoint), FCanExecuteAction::CreateSP(this, &FAIPointContextEdModeToolkit::CanAddPoint)));

	CommandList->MapAction(Commands.LinkPoints,
		FUIAction(FExecuteAction::CreateSP(this, &FAIPointContextEdModeToolkit::OnLinkPoints), FCanExecuteAction::CreateSP(this, &FAIPointContextEdModeToolkit::CanLinkPoints)));

	CommandList->MapAction(Commands.ClearLinks,
		FUIAction(FExecuteAction::CreateSP(this, &FAIPointContextEdModeToolkit::OnClearLinks), FCanExecuteAction::CreateSP(this, &FAIPointContextEdModeToolkit::CanClearLinks)));

	CommandList->MapAction(Commands.DeletePoints,
		FUIAction(FExecuteAction::CreateSP(this, &FAIPointContextEdModeToolkit::OnRemovePoints), FCanExecuteAction::CreateSP(this, &FAIPointContextEdModeToolkit::CanRemovePoints)));

	CommandList->MapAction(Commands.AddSection,
		FUIAction(FExecuteAction::CreateSP(this, &FAIPointContextEdModeToolkit::OnAddSection), FCanExecuteAction::CreateSP(this, &FAIPointContextEdModeToolkit::CanAddSection)));

	CommandList->MapAction(Commands.RemoveSection,
		FUIAction(FExecuteAction::CreateSP(this, &FAIPointContextEdModeToolkit::OnRemoveSection), FCanExecuteAction::CreateSP(this, &FAIPointContextEdModeToolkit::CanRemoveSection)));
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
	ToolbarBuilder.AddToolBarButton(Commands.LinkPoints);
	ToolbarBuilder.AddToolBarButton(Commands.ClearLinks);
	ToolbarBuilder.AddToolBarButton(Commands.DeletePoints);
	ToolbarBuilder.AddSeparator();
	ToolbarBuilder.AddToolBarButton(Commands.AddSection);
	ToolbarBuilder.AddToolBarButton(Commands.RemoveSection);
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
	FAIPointContextEdMode* EdMode = GetEditorMode();
	if (EdMode)
	{
		return EdMode->GetSelectedTargetPointActor() != nullptr;
	}

	return false;
}

void FAIPointContextEdModeToolkit::OnLinkPoints()
{
	FAIPointContextEdMode* EdMode = GetEditorMode();
	if (EdMode)
	{
		EdMode->CreateLink();
	}
}

bool FAIPointContextEdModeToolkit::CanLinkPoints() const
{
	FAIPointContextEdMode* EdMode = GetEditorMode();
	if (EdMode)
	{
		return EdMode->CanCreateLink();
	}

	return false;
}

void FAIPointContextEdModeToolkit::OnClearLinks()
{
	FAIPointContextEdMode* EdMode = GetEditorMode();
	if (EdMode)
	{
		return EdMode->ClearLinks();
	};
}

bool FAIPointContextEdModeToolkit::CanClearLinks() const
{
	FAIPointContextEdMode* EdMode = GetEditorMode();
	if (EdMode)
	{
		return EdMode->CanClearLinks();
	}

	return false;
}

void FAIPointContextEdModeToolkit::OnRemovePoints()
{
	FAIPointContextEdMode* EdMode = GetEditorMode();
	if (EdMode)
	{
		EdMode->RemovePoints();
	}
	
}

bool FAIPointContextEdModeToolkit::CanRemovePoints() const
{
	FAIPointContextEdMode* EdMode = GetEditorMode();
	if (EdMode)
	{
		return EdMode->CanRemovePoints();
	}
	return false;
}

void FAIPointContextEdModeToolkit::OnAddSection()
{
	FAIPointContextEdMode* EdMode = GetEditorMode();
	if (EdMode)
	{
		EdMode->AddSection();
	}
}

bool FAIPointContextEdModeToolkit::CanAddSection() const
{
	FAIPointContextEdMode* EdMode = GetEditorMode();
	if (EdMode)
	{
		return EdMode->CanAddSection();
	}

	return false;
}

void FAIPointContextEdModeToolkit::OnRemoveSection()
{
	FAIPointContextEdMode* EdMode = GetEditorMode();
	if (EdMode)
	{
		EdMode->RemoveSection();
	}
}

bool FAIPointContextEdModeToolkit::CanRemoveSection() const
{
	FAIPointContextEdMode* EdMode = GetEditorMode();
	if (EdMode)
	{
		return EdMode->CanRemoveSection();
	}

	return false;
}
