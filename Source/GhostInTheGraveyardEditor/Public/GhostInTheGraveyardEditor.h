#pragma once

// #include "UnrealEd.h"
// #include "SlateBasics.h"
// #include "SlateExtras.h"
// #include "Editor/LevelEditor/Public/LevelEditor.h"
// #include "Editor/PropertyEditor/Public/PropertyEditing.h"
// #include "IAssetTypeActions.h"
#include "Modules/ModuleInterface.h"

class FGhostInTheGraveyardEditor : public IModuleInterface
{

public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual void AddModuleListeners();

protected:
	TArray<TSharedRef<IModuleInterface>> ModuleListeners;
};