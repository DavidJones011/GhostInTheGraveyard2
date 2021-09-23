
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "AIPointContextEdMode.h"
#include "AIPointContextEditorObject.generated.h"

UCLASS(MinimalAPI)
class UAIPointContextEditorObject : public UObject
{
	GENERATED_UCLASS_BODY()

	FAIPointContextEdMode* ParentMode;

	/*How spread out each search point should be generated.*/
	UPROPERTY(Category = "Search Point Generation", EditAnywhere, meta=(DisplayName="Spread", ShowForTools="SearchPointSpread"))
	float GenerationSpread;

	/*The extent of the area search points can be generated.*/
	UPROPERTY(Category = "Search Point Generation", EditAnywhere, meta=(DisplayName = "Extent", ShowForTools = "SearchPointExtent"))
	float GenerationExtent;

	/*The maximum height search points can be generated.*/
	UPROPERTY(Category = "Search Point Generation", EditAnywhere, meta = (DisplayName = "Height", ShowForTools = "SearchPointHeight"))
	float GenerationMaxHeight;

	/*The threshold of the slope to generate a point on.*/
	UPROPERTY(Category = "Search Point Generation", EditAnywhere, meta = (DisplayName = "Slope Threshold", ShowForTools = "SearchPointSlopeThreshold"))
	float GenerationSlopeThreshold;

	/*The radius of the drawn debug spheres.*/
	UPROPERTY(Category = "Debug", EditAnywhere, meta = (DisplayName = "Sphere Radius", ShowForTools = "DebugSphereRadius"))
	float DebugSphereRadius;

	/*Hides all points of 'search' type from viewport when true.*/
	UPROPERTY(Category = "Debug", EditAnywhere, meta = (DisplayName = "Hide Search Points", ShowForTools = "HideSearachPoints"))
		bool bHiddenSearchPoints;

	/*Hides all points of 'sound transfer' type from viewport when true.*/
	UPROPERTY(Category = "Debug", EditAnywhere, meta = (DisplayName = "Hide Sound Transfer Points", ShowForTools = "HideSoundPoints"))
	bool bHiddenSoundTransferPoints;

	/*Hides all points of 'patrol' type from viewport when true.*/
	UPROPERTY(Category = "Debug", EditAnywhere, meta = (DisplayName = "Hide Patrol Points", ShowForTools = "HidePatrolPoints"))
	bool bHiddenPatrolPoints;

public:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	void SetParent(FAIPointContextEdMode* Parent)
	{
		ParentMode = Parent;
	}
};