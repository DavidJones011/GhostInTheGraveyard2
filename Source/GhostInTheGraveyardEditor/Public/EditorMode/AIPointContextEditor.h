#include "Modules/ModuleInterface.h"

class FAIPointContextEditor : public IModuleInterface
{

public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	static TSharedPtr<class FSlateStyleSet> StyleSet;

	void RegisterStyleSet();
	void UnregisterStyleSet();

	void RegisterEditorMode();
	void UnregisterEditorMode();
};