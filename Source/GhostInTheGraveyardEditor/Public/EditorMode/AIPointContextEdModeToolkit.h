
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
		UI_COMMAND(AddPoint, "Add Point", "Add a point.",
			EUserInterfaceActionType::Button, FInputChord());

		UI_COMMAND(LinkPoints, "Link Points", "Link two points.", 
			EUserInterfaceActionType::Button, FInputChord(EKeys::L));
	}

#undef LOCTEXT_NAMESPACE

	//TSharedPtr<FUICommandInfo> DeletePoint;
	TSharedPtr<FUICommandInfo> AddPoint;
	TSharedPtr<FUICommandInfo> LinkPoints;
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

private:

	const static TArray<FName> PaletteNames;
	TSharedPtr<SAIPointContextEdModeWidget> AIPointContextEdModeWidget;
};
