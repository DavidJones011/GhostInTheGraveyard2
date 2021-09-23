
#include "EditorMode/AIPointContextEditorObject.h"

UAIPointContextEditorObject::UAIPointContextEditorObject(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
, GenerationSpread(100.0F)
, GenerationExtent(10000.0F)
, GenerationMaxHeight(500.0F)
, GenerationSlopeThreshold(0.4)
, DebugSphereRadius(30.0F)
, DebugDrawDistance(30000.0F)
{
	
}

#if WITH_EDITOR
void UAIPointContextEditorObject::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FProperty* Property = PropertyChangedEvent.MemberProperty;
	FName PropertyName;
	if (Property)
	{
		PropertyName = Property->GetFName();
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UAIPointContextEditorObject, GenerationSpread))
	{
		GenerationSpread = FMath::Clamp<float>(GenerationSpread, 50.0F, WORLD_MAX);
		if(ParentMode)
			ParentMode->GenerateSearchPoints(GenerationSpread, GenerationExtent, GenerationMaxHeight, GenerationSlopeThreshold);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UAIPointContextEditorObject, GenerationExtent))
	{
		GenerationExtent = FMath::Clamp<float>(GenerationExtent, 50.0F, WORLD_MAX);
		if (ParentMode)
			ParentMode->GenerateSearchPoints(GenerationSpread, GenerationExtent, GenerationMaxHeight, GenerationSlopeThreshold);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UAIPointContextEditorObject, GenerationMaxHeight))
	{
		GenerationMaxHeight = FMath::Clamp<float>(GenerationMaxHeight, 50.0F, WORLD_MAX);
		if (ParentMode)
			ParentMode->GenerateSearchPoints(GenerationSpread, GenerationExtent, GenerationMaxHeight, GenerationSlopeThreshold);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UAIPointContextEditorObject, GenerationSlopeThreshold))
	{
		GenerationSlopeThreshold = FMath::Clamp<float>(GenerationSlopeThreshold, 0.0F, 1.0F);
		if (ParentMode)
			ParentMode->GenerateSearchPoints(GenerationSpread, GenerationExtent, GenerationMaxHeight, GenerationSlopeThreshold);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UAIPointContextEditorObject, DebugSphereRadius))
	{
		DebugSphereRadius = FMath::Clamp<float>(DebugSphereRadius, 5.0F, 600.0F);
		if (ParentMode)
			ParentMode->SetDebugSphereRadius(DebugSphereRadius);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UAIPointContextEditorObject, bHiddenSearchPoints))
	{
		if (ParentMode)
			ParentMode->SetHiddenDebugSpheresByType(bHiddenSearchPoints, EPointType::Search);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UAIPointContextEditorObject, bHiddenSoundTransferPoints))
	{
		if (ParentMode)
			ParentMode->SetHiddenDebugSpheresByType(bHiddenSearchPoints, EPointType::SoundTransfer);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UAIPointContextEditorObject, bHiddenPatrolPoints))
	{
		if (ParentMode)
			ParentMode->SetHiddenDebugSpheresByType(bHiddenSearchPoints, EPointType::Patrol);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UAIPointContextEditorObject, DebugDrawDistance))
	{
		if (ParentMode)
			ParentMode->SetDebugDrawDistance(DebugDrawDistance);
	}
}
#endif
