
#pragma once

#include "Toolkits/BaseToolkit.h"
#include "EditorMode/AIPointContextEdMode.h"
#include "EditorMode/SAIPointContextEdModeWidget.h"
#include "EditorModeManager.h"

class FAIPointContextEdModeToolkit : public FModeToolkit
{

public:
	FAIPointContextEdModeToolkit()
	{
		SAssignNew(AIPointContextEdModeWidget, SAIPointContextEdModeWidget);
	}

	virtual FName GetToolkitFName() const override { return FName("AIPointContextEdMode"); }
	virtual FText GetBaseToolkitName() const override { return NSLOCTEXT("BuilderModeToolkit", "DisplayName", "Builder"); }
	virtual class FEdMode* GetEditorMode() const override { return GLevelEditorModeTools().GetActiveMode(FAIPointContextEdMode::EM_AIPointContext); }
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return AIPointContextEdModeWidget; }

private:

	TSharedPtr<SAIPointContextEdModeWidget> AIPointContextEdModeWidget;
};