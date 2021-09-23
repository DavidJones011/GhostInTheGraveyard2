
#include "EditorMode/AIPointContextEditor.h"
#include "Modules/ModuleManager.h"
#include "GameDebuggers/GameplayDebuggerCategory_Creature.h"
#include "GameplayDebugger.h"
#include "EditorMode/AIPointContextEdMode.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

TSharedPtr<FSlateStyleSet> FAIPointContextEditor::StyleSet = nullptr;

void FAIPointContextEditor::StartupModule()
{
	// register editor modes and slate style sets
	RegisterStyleSet();
	RegisterEditorMode();
	
}

void FAIPointContextEditor::ShutdownModule()
{
	// unregister editor modes and slate styles
	UnregisterEditorMode();
	UnregisterStyleSet();
}

void FAIPointContextEditor::RegisterStyleSet()
{
	// icon sizes
	const FVector2D Icon20x20(20.0F, 20.0F);
	const FVector2D Icon40x40(40.0F, 40.0F);

	// register the style set only once
	if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShareable(new FSlateStyleSet("AIPointContextEdToolStyle"));
	StyleSet->SetContentRoot(FPaths::ProjectDir() / TEXT("Content/EditorResources"));
	StyleSet->SetCoreContentRoot(FPaths::ProjectDir() / TEXT("Content/EditorResources"));

	// point editor
	{
		StyleSet->Set("AIPointContextEdMode", new IMAGE_BRUSH(TEXT("IconAIPointContextEditorMode"), Icon40x40));
		StyleSet->Set("AIPointContextEdMode.Small", new IMAGE_BRUSH(TEXT("IconAIPointContextEditorMode"), Icon20x20));
	}

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FAIPointContextEditor::UnregisterStyleSet()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

void FAIPointContextEditor::RegisterEditorMode()
{
	FEditorModeRegistry::Get().RegisterMode<FAIPointContextEdMode>(
		FAIPointContextEdMode::EM_AIPointContext,
		FText::FromString("AI Point Context Editor"),
		FSlateIcon(StyleSet->GetStyleSetName(), "AIPointContextEdMode", "AIPointContextEdMode.Small"),
		true, 500
		);
}

void FAIPointContextEditor::UnregisterEditorMode()
{
	FEditorModeRegistry::Get().UnregisterMode(FAIPointContextEdMode::EM_AIPointContext);
}

#undef IMAGE_BRUSH