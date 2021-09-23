
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

	HAIPointContextProxy(UObject* InRefObject, int32 InIndex)
		: HHitProxy(HPP_UI), RefObject(InRefObject), Index(InIndex)
	{
	}

	UObject* RefObject;
	EPointType Type;
	int32 Index;
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

	void MapCommands();
	virtual bool InputKey(FEditorViewportClient* ViewportClient,FViewport* Viewport,FKey Key,EInputEvent Event);
	TSharedPtr<SWidget> GenerateContextMenu(FEditorViewportClient* InViewportClient) const;

	AAIPointContextManager* GetSelectedTargetPointActor() const;
	void AddPoint(EPointType PointType);
	void AddPointAtLocation(FVector Location, EPointType PointType);
	bool CanAddPoint(EPointType PointType) const;
	void RemovePoint();
	bool CanRemovePoint();
	bool HasValidSelection() const;
	void SelectPoint(AAIPointContextManager* Actor, int32 Index);

	void SetDebugSphereRadius(float Radius);
	float GetDebugSphereRadius() const { return DebugSphereRadius; }
	void SetHiddenDebugSpheresByType(bool Hidden, EPointType PointType);

	void GenerateSearchPoints(float Spread, float Extent, float MaxHeight, float SlopeThreshold);
	void ClearSearchPoints();

	TWeakObjectPtr<AAIPointContextManager> CurrentSelectedTarget;
	EPointType CurrentSelectedPointType;
	int32 CurrentSelectedIndex = -1;

	TSharedPtr<FUICommandList> AIPointContextEdModeActions;

public:

	class UAIPointContextEditorObject* UISettings;

private:

	float DebugSphereRadius = 10.0F;
	bool bHiddenSearchPoints = false;
	bool bHiddenSoundTransferPoints = false;
	bool bHiddenPatrolPoints = false;
	TArray<FColor, TInlineAllocator<3>> SphereColors = { FColor(51,255,51), FColor(255, 51, 153), FColor(255,255,51)};
};