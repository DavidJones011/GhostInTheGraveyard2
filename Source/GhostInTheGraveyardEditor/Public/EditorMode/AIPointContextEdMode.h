
#pragma once

#include "EditorModes.h"
#include "EdMode.h"
#include "AI/AIPointContextManager.h"

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
	void RemovePoint();
	bool CanRemovePoint();
	bool HasValidSelection() const;
	void SelectPoint(AAIPointContextManager* Actor, int32 Index, EPointType PointType);

	void SetDebugSphereRadius(float Radius);
	void SetDebugDrawDistance(float Distance);
	float GetDebugSphereRadius() const { return DebugSphereRadius; }
	void SetHiddenDebugSpheresByType(bool Hidden, EPointType PointType);

	void GenerateSearchPoints(float Spread, float Extent, float MaxHeight, float SlopeThreshold);
	void ClearSearchPoints();

	TSharedRef<FUICommandList> GetUICommandList() const;

public:

	class UAIPointContextEditorObject* UISettings;
	TWeakObjectPtr<AAIPointContextManager> CurrentSelectedTarget;
	TSharedPtr<FUICommandList> AIPointContextEdModeActions;
	EPointType CurrentSelectedPointType = EPointType::Search;
	int32 CurrentSelectedIndex = -1;
	int32 CurrentPatrolSection = -1;

private:

	float DebugSphereRadius = 10.0F;
	bool bHiddenSearchPoints = false;
	bool bHiddenSoundTransferPoints = false;
	bool bHiddenPatrolPoints = false;
	float DebugRenderDistance = 800.0F;
	TArray<FColor, TInlineAllocator<3>> SphereColors = { FColor(51,255,51), FColor(255, 51, 153), FColor(255,255,51)};
};