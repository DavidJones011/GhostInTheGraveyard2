
#include "AI/AIPointContextManager.h"
#include "Containers/Array.h"

AAIPointContextManager::AAIPointContextManager()
{
}

int32 AAIPointContextManager::CreatePatrolSection(FVector FirstPoint)
{
	int32 NewID = PatrolSections.Num();

	// create section
	FPatrolSection PatrolSection;
	PatrolSection.SectionID = NewID;

	// create first point
	FPatrolPointData PatrolPointData;
	PatrolPointData.SectionId = NewID;
	PatrolPointData.Location = FirstPoint;
	PatrolPointData.Index = 0;

	// add new point to section
	PatrolSection.PatrolPoints.Add(PatrolPointData);
	PatrolSections.Add(PatrolSection);

	return NewID;
}

int32 AAIPointContextManager::AddPatrolPointToSection(FVector Point, int32 SectionID)
{
	if(!PatrolSections.IsValidIndex(SectionID))
		return -1;

	TArray<FPatrolPointData>& PatrolData = PatrolSections[SectionID].PatrolPoints;
	const int32 NewIndex = PatrolData.Num();

	FPatrolPointData PatrolPointData;
	PatrolPointData.SectionId = SectionID;
	PatrolPointData.Index = NewIndex;
	PatrolPointData.Location = Point;
	PatrolData.Add(PatrolPointData);

	return NewIndex;
}

void AAIPointContextManager::LinkPatrolPoints(int32 FromPointIndex, int32 ToPointIndex, int32 Section, bool bLinkBothWays /*= false*/)
{
	// check if the section and index are valid
	if(!IsValid(Section, FromPointIndex) || !IsValid(Section, ToPointIndex))
		return;

	// doesn't make much sense to create a link to itself
	if(FromPointIndex == ToPointIndex)
		return;

	FPatrolPointData& FromData = PatrolSections[Section].PatrolPoints[FromPointIndex];
	FPatrolPointData& ToData = PatrolSections[Section].PatrolPoints[ToPointIndex];

	FromData.NextLinkIndex = ToPointIndex;

	if (bLinkBothWays)
	{
		ToData.PriorLinkIndex = FromPointIndex;
	}
}

void AAIPointContextManager::RemoveLink(int32 PointIndex, int32 Section, EUnlinkPoints UnlinkType)
{
	// check if the section and index are valid
	if (!IsValid(Section, PointIndex))
		return;

	FPatrolPointData& Data = PatrolSections[Section].PatrolPoints[PointIndex];
	if (UnlinkType == EUnlinkPoints::Unlink_Next || UnlinkType == EUnlinkPoints::Unlink_Both)
	{
		FPatrolPointData NextData;
		if (TryGetPatrolPointData(Data.NextLinkIndex, Section, NextData))
		{
			if (NextData.PriorLinkIndex == PointIndex)
			{
				NextData.PriorLinkIndex = -1;
			}
		}
		Data.NextLinkIndex = -1;
	}

	if (UnlinkType == EUnlinkPoints::Unlink_Prior || UnlinkType == EUnlinkPoints::Unlink_Both)
	{
		FPatrolPointData PriorData;
		if (TryGetPatrolPointData(Data.PriorLinkIndex, Section, PriorData))
		{
			if (PriorData.NextLinkIndex == PointIndex)
			{
				PriorData.NextLinkIndex = -1;
			}
		}
		Data.PriorLinkIndex = -1;
	}
}

bool AAIPointContextManager::IsValid(int32 SectionID, int32 PointIndex) const
{
	if (PatrolSections.IsValidIndex(SectionID))
	{
		return PatrolSections[SectionID].PatrolPoints.IsValidIndex(PointIndex);
	}

	return false;
}

bool AAIPointContextManager::TryGetPatrolPointData(int32 PointIndex, int32 SectionID, FPatrolPointData& Data) const
{
	if (IsValid(SectionID, PointIndex))
	{
		Data = PatrolSections[SectionID].PatrolPoints[PointIndex];
		return true;
	}

	return false;
}

const TArray<FPatrolPointData>* AAIPointContextManager::GetPatrolPointData(int32 SectionID) const
{
	if (PatrolSections.IsValidIndex(SectionID))
	{
		return &PatrolSections[SectionID].PatrolPoints;
	}
	return nullptr;
}

FVector* AAIPointContextManager::GetPatrolPointVectorPtr(int32 SectionID, int32 PointIndex)
{
	return IsValid(SectionID, PointIndex) ? &PatrolSections[SectionID].PatrolPoints[PointIndex].Location : nullptr;
}

void AAIPointContextManager::BeginPlay()
{
}
