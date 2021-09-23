
#include "GhostInTheGraveyardEditor.h"
#include "Modules/ModuleManager.h"
#include "GameDebuggers/GameplayDebuggerCategory_Creature.h"
#include "GameplayDebugger.h"
#include "EditorMode/AIPointContextEdMode.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "EditorMode/AIPointContextEditor.h"

IMPLEMENT_GAME_MODULE(FGhostInTheGraveyardEditor, GhostInTheGraveyardEditor)

void FGhostInTheGraveyardEditor::StartupModule()
{
	// set up gameplay debugger categories
	IGameplayDebugger& GameplayDebuggerModule =  IGameplayDebugger::Get();
	GameplayDebuggerModule.RegisterCategory("CreatureAI"
		, IGameplayDebugger::FOnGetCategory::CreateStatic(&FGameplayDebuggerCategory_Creature::MakeInstance)
		, EGameplayDebuggerCategoryState::EnabledInGameAndSimulate);

	GameplayDebuggerModule.NotifyCategoriesChanged();

	// run child modules
	if (!IsRunningCommandlet())
	{
		AddModuleListeners();
		for (int32 i = 0; i < ModuleListeners.Num(); ++i)
		{
			ModuleListeners[i]->StartupModule();
		}
	}
}

void FGhostInTheGraveyardEditor::ShutdownModule()
{
	// unregister gameplay debugger categories
	if (IGameplayDebugger::IsAvailable())
	{
		IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
		GameplayDebuggerModule.UnregisterCategory("CreatureAI");
	}

	// shutdown child modules
	for (int32 i = 0; i < ModuleListeners.Num(); ++i)
	{
		ModuleListeners[i]->ShutdownModule();
	}
}

void FGhostInTheGraveyardEditor::AddModuleListeners()
{
	ModuleListeners.Add(MakeShareable(new FAIPointContextEditor));
}