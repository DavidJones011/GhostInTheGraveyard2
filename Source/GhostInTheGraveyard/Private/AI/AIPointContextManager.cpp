
#include "AI/AIPointContextManager.h"
#include "Containers/Array.h"
#include "AI/AIDirectorSubsystem.h"

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

void AAIPointContextManager::RemovePatrolPointFromSection(int32 Index, int32 Section)
{
	if(!IsValid(Section, Index))
		return;

	RemoveLink(Index, Section, Unlink_Both);
	PatrolSections[Section].PatrolPoints.RemoveAt(Index);

	if (PatrolSections[Section].PatrolPoints.Num() == 0)
	{
		PatrolSections.RemoveAt(Section);
		return;
	}

	// update the indicies
	int32 PatrolPointNum = PatrolSections[Section].PatrolPoints.Num();
	for (int32 UpdateIndex = 0; UpdateIndex < PatrolPointNum; UpdateIndex++)
	{
		FPatrolPointData& Data = PatrolSections[Section].PatrolPoints[UpdateIndex];
		if (Data.Index >= Index)
		{
			Data.Index--;
		}

		if (Data.PriorLinkIndex != -1 && Data.PriorLinkIndex >= Index)
		{
			Data.PriorLinkIndex--;
			Data.PriorLinkIndex = (Data.PriorLinkIndex == Data.Index) ? -1 : Data.PriorLinkIndex;
		}
		if (Data.NextLinkIndex != -1 && Data.NextLinkIndex >= Index)
		{
			Data.NextLinkIndex--;
			Data.NextLinkIndex = (Data.NextLinkIndex == Data.Index) ? -1 : Data.NextLinkIndex;
		}
	}
}

void AAIPointContextManager::RemovePatrolSection(int32 Section)
{
	PatrolSections.RemoveAt(Section);

	// update the section ids
	int32 PatrolSectNum = PatrolSections.Num();
	for (int32 UpdateSection = 0; UpdateSection < PatrolSectNum; UpdateSection++)
	{
		FPatrolSection& PatrolSection = PatrolSections[UpdateSection];
		if (PatrolSection.SectionID >= Section)
		{
			PatrolSection.SectionID--;
		}

		int32 PatrolPointNum = PatrolSection.PatrolPoints.Num();
		for (int32 UpdateIndex = 0; UpdateIndex < PatrolPointNum; UpdateIndex++)
		{
			FPatrolPointData& Data = PatrolSection.PatrolPoints[UpdateIndex];
			Data.SectionId = PatrolSection.SectionID;
		}
	}
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
		if (IsValid(Section, Data.NextLinkIndex))
		{
			int32& PriorIndex = PatrolSections[Section].PatrolPoints[Data.NextLinkIndex].PriorLinkIndex;
			if (PriorIndex == PointIndex)
			{
				PriorIndex = -1;
			}
		}
		Data.NextLinkIndex = -1;
	}

	if (UnlinkType == EUnlinkPoints::Unlink_Prior || UnlinkType == EUnlinkPoints::Unlink_Both)
	{
		if (IsValid(Section, Data.PriorLinkIndex))
		{
			int32& NextIndex = PatrolSections[Section].PatrolPoints[Data.PriorLinkIndex].NextLinkIndex;
			if (NextIndex == PointIndex)
			{
				NextIndex = -1;
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

bool AAIPointContextManager::TryGetPatrolPointNextData(int32 PointIndex, int32 SectionID, FPatrolPointData& Data) const
{
	if (IsValid(SectionID, PointIndex))
	{
		int32 NextIndex = PatrolSections[SectionID].PatrolPoints[PointIndex].NextLinkIndex;
		if (IsValid(SectionID, NextIndex))
		{
			Data = PatrolSections[SectionID].PatrolPoints[NextIndex];
			return true;
		}
	}

	return false;
}

bool AAIPointContextManager::TryGetPatrolPointPriorData(int32 PointIndex, int32 SectionID, FPatrolPointData& Data) const
{
	if (IsValid(SectionID, PointIndex))
	{
		int32 PriorIndex = PatrolSections[SectionID].PatrolPoints[PointIndex].PriorLinkIndex;
		if (IsValid(SectionID, PriorIndex))
		{
			Data = PatrolSections[SectionID].PatrolPoints[PriorIndex];
			return true;
		}
	}

	return false;
}

bool AAIPointContextManager::TryGetClosestPatrolPointData(const FVector& Point, FPatrolPointData& Data)
{
	// this could be improved, bsp tree would work well
	FPatrolPointData* BestData = nullptr;
	float BestDistance = FLT_MAX;

	for (int32 Section = 0; Section < PatrolSections.Num(); Section++)
	{
		const FPatrolSection& SectionData = PatrolSections[Section];
		for (int32 Index = 0; Index < SectionData.PatrolPoints.Num(); Index++)
		{
			const FPatrolPointData* CurrentData = &(SectionData.PatrolPoints[Index]);
			float DistSquared = FVector::DistSquared(Point, CurrentData->Location);
			if (DistSquared < BestDistance)
			{
				BestDistance = DistSquared;
				BestData = (FPatrolPointData*)CurrentData;
			}
		}
	}

	if (BestData != nullptr)
	{
		Data = *BestData;
		return true;
	}

	Data.Index = -1;
	Data.NextLinkIndex = -1;
	Data.PriorLinkIndex = -1;
	Data.Location = FVector::ZeroVector;
	Data.SectionId = -1;
	return false;
}

bool AAIPointContextManager::TryGetClosestPatrolPointDataFromSection(int32 Section, const FVector& Point, FPatrolPointData& Data)
{
	FPatrolPointData* BestData = nullptr;
	float BestDistance = FLT_MAX;

	if(IsValidSection(Section))
	{
		const FPatrolSection& SectionData = PatrolSections[Section];
		for (int32 Index = 0; Index < SectionData.PatrolPoints.Num(); Index++)
		{
			const FPatrolPointData* CurrentData = &(SectionData.PatrolPoints[Index]);
			float DistSquared = FVector::DistSquared(Point, CurrentData->Location);
			if (DistSquared < BestDistance)
			{
				BestDistance = DistSquared;
				BestData = (FPatrolPointData*)CurrentData;
			}
		}

		if (BestData != nullptr)
		{
			Data = *BestData;
			return true;
		}
	}

	Data.Index = -1;
	Data.NextLinkIndex = -1;
	Data.PriorLinkIndex = -1;
	Data.Location = FVector::ZeroVector;
	Data.SectionId = -1;
	return false;
}

bool AAIPointContextManager::TryGetFarthestPatrolPointDataFromSection(int32 Section, const FVector& Point, FPatrolPointData& Data)
{
	FPatrolPointData* BestData = nullptr;
	float BestDistance = 0.0F;

	if (IsValidSection(Section))
	{
		const FPatrolSection& SectionData = PatrolSections[Section];
		for (int32 Index = 0; Index < SectionData.PatrolPoints.Num(); Index++)
		{
			const FPatrolPointData* CurrentData = &(SectionData.PatrolPoints[Index]);
			float DistSquared = FVector::DistSquared(Point, CurrentData->Location);
			if (DistSquared > BestDistance)
			{
				BestDistance = DistSquared;
				BestData = (FPatrolPointData*)CurrentData;
			}
		}

		if (BestData != nullptr)
		{
			Data = *BestData;
			return true;
		}
	}

	Data.Index = -1;
	Data.NextLinkIndex = -1;
	Data.PriorLinkIndex = -1;
	Data.Location = FVector::ZeroVector;
	Data.SectionId = -1;
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
	UWorld* World = GetWorld();
	if (World)
	{
		UAIDirectorSubsystem* Director = GetWorld()->GetSubsystem<UAIDirectorSubsystem>();
		Director->RegisterAIContextManager(this);
	}
}
