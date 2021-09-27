
#pragma once

#include "EditorModes.h"
#include "EdMode.h"
#include "AI/AIPointContextManager.h"

class UAIPointContextEditorObject;

/**
 * Hit proxy so that points can be manipulated through the viewport editor.
 */
struct HAIPointContextProxy : public HHitProxy
{
	DECLARE_HIT_PROXY();

	HAIPointContextProxy(UObject* InRefObject, int32 InIndex, EPointType InPointType, int32 InPatrolSection)
		: HHitProxy(HPP_UI), RefObject(InRefObject), Index(InIndex), PointType(InPointType), PatrolSection(InPatrolSection)
	{
	}

	UObject* RefObject;
	int32 Index;
	EPointType PointType;
	int32 PatrolSection;
};

struct SelectionData
{
	TWeakObjectPtr<AAIPointContextManager> CurrentSelectedTarget;
	EPointType CurrentSelectedPointType = EPointType::Search;
	int32 CurrentSelectedIndex = -1;
	int32 Section = -1;
};

class FAIPointContextEdMode : public FEdMode 
{
public:

	FAIPointContextEdMode();
	~FAIPointContextEdMode();

	const static FEditorModeID EM_AIPointContext;

	virtual void Enter() override;
	virtual void Exit() override;

	/* Performs the rendering in the viewport */
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI);
	/* Handles clicking on items in viewport */
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click);

	/* Functions that handle the transform widget */
	virtual bool InputDelta(FEditorViewportClient* InViewportClient,FViewport* InViewport,FVector& InDrag,FRotator& InRot,FVector& InScale) override;
	virtual bool ShowModeWidgets() const override { return true; }
	virtual bool ShouldDrawWidget() const override { return true; }
	virtual bool UsesTransformWidget() const override { return true; }
	virtual FVector GetWidgetLocation() const override;

	virtual bool InputKey(FEditorViewportClient* ViewportClient,FViewport* Viewport,FKey Key,EInputEvent Event);
	TSharedPtr<SWidget> GenerateContextMenu(FEditorViewportClient* InViewportClient) const;

	AAIPointContextManager* GetSelectedTargetPointActor() const;
	void AddPoint(EPointType PointType);
	void AddPointAtLocation(FVector Location, EPointType PointType);
	bool CanAddPoint(EPointType PointType) const;

	void RemovePoints();
	bool CanRemovePoints() const;

	bool HasValidSelection() const;
	void SelectPoint(AAIPointContextManager* Actor, int32 Index, EPointType PointType, int32 Section = -1);

	void CreateLink();
	bool CanCreateLink() const;

	void ClearLinks();
	bool CanClearLinks() const;

	void SetDebugSphereRadius(float Radius);
	void SetDebugDrawDistance(float Distance);
	float GetDebugSphereRadius() const { return DebugSphereRadius; }
	void SetHiddenDebugSpheresByType(bool Hidden, EPointType PointType);

	void GenerateSearchPoints(float Spread, float Extent, float MaxHeight, float SlopeThreshold);
	void ClearSearchPoints();

	bool SelectionSharesSame(bool bCompareSection = false) const;

	bool IsAlreadySelected(const SelectionData& InData, int32& OutIndex);
	int32 SelectionNum() const { return Selection.Num(); }

	TSharedRef<FUICommandList> GetUICommandList() const;
	UAIPointContextEditorObject* GetUISettings() const { return UISettings; }

protected:

	UAIPointContextEditorObject* UISettings;
	TSharedPtr<FUICommandList> AIPointContextEdModeActions;
	TArray<SelectionData> Selection;

	bool bIsMultiSelecting = false;

private:

	float DebugSphereRadius = 10.0F;
	bool bHiddenSearchPoints = false;
	bool bHiddenSoundTransferPoints = false;
	bool bHiddenPatrolPoints = false;
	float DebugRenderDistance = 800.0F;
	TArray<FColor, TInlineAllocator<3>> SphereColors = { FColor(51,255,51), FColor(255, 51, 153), FColor(255,255,51)};
};