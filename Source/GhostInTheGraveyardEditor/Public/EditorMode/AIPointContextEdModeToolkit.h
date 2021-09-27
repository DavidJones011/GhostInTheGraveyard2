
#pragma once

#include "Toolkits/BaseToolkit.h"
#include "EditorMode/AIPointContextEdMode.h"
#include "EditorMode/SAIPointContextEdModeWidget.h"
#include "EditorModeManager.h"

class SWidget;
class EdMode;

/* class for setting up keyboard input */
class FAIPointContextEditorCommands : public TCommands<FAIPointContextEditorCommands>
{
public:

	FAIPointContextEditorCommands()
		: TCommands<FAIPointContextEditorCommands>("AIPointContextEditor",
			FText::FromString(TEXT("AI Point Context Editor")), NAME_None, FEditorStyle::GetStyleSetName())
	{
	}

#define LOCTEXT_NAMESPACE ""

	virtual void RegisterCommands() override
	{
		UI_COMMAND(AddPoint, "Add Point", "Adds a point.",
			EUserInterfaceActionType::Button, FInputChord());

		UI_COMMAND(LinkPoints, "Link Points", "Links points in selected order.", 
			EUserInterfaceActionType::Button, FInputChord(EKeys::L));

		UI_COMMAND(ClearLinks, "Clear Link(s)", "Clears the links of selected points.",
			EUserInterfaceActionType::Button, FInputChord(EKeys::U));

		UI_COMMAND(DeletePoints, "Delete Point(s)", "Deletes selected points.",
			EUserInterfaceActionType::Button, FInputChord(EKeys::Delete));
	}

#undef LOCTEXT_NAMESPACE

	//TSharedPtr<FUICommandInfo> DeletePoint;
	TSharedPtr<FUICommandInfo> AddPoint;
	TSharedPtr<FUICommandInfo> LinkPoints;
	TSharedPtr<FUICommandInfo> ClearLinks;
	TSharedPtr<FUICommandInfo> DeletePoints;
};

class FAIPointContextEdModeToolkit : public FModeToolkit
{

public:
	/** Initializes the geometry mode toolkit */
	virtual void Init(const TSharedPtr< class IToolkitHost >& InitToolkitHost) override;

	void MapCommands();

	virtual FName GetToolkitFName() const override { return FName("AIPointContextEdMode"); }
	virtual FText GetBaseToolkitName() const override { return NSLOCTEXT("BuilderModeToolkit", "DisplayName", "Builder"); }
	virtual class FAIPointContextEdMode* GetEditorMode() const override;
	virtual TSharedPtr<SWidget> GetInlineContent() const override { return AIPointContextEdModeWidget; }

	virtual void GetToolPaletteNames(TArray<FName>& InPaletteName) const;
	virtual void BuildToolPalette(FName PaletteName, class FToolBarBuilder& ToolbarBuilder);

	bool GetIsPropertyVisibleFromProperty(const FProperty& Property) const;

	void OnAddPoint();
	bool CanAddPoint() const;

	void OnLinkPoints();
	bool CanLinkPoints() const;

	void OnClearLinks();
	bool CanClearLinks() const;

	void OnRemovePoints();
	bool CanRemovePoints() const;

private:

	const static TArray<FName> PaletteNames;
	TSharedPtr<SAIPointContextEdModeWidget> AIPointContextEdModeWidget;
};
