// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PatrolTrackerComponent.h"
#include "AI/AIPointContextManager.h"
#include "EngineUtils.h"

// Sets default values for this component's properties
UPatrolTrackerComponent::UPatrolTrackerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UPatrolTrackerComponent::BeginPlay()
{
	Super::BeginPlay();
	SetTraversalMode(DefaultMode);
}

void UPatrolTrackerComponent::TraverseLoop(const FPatrolPointData& Data, bool bReverse /* = false*/)
{
	if (TrackedPatrolPathManager)
	{
		FPatrolPointData ChosenData;
		if (bReverse)
		{
			if (TrackedPatrolPathManager->TryGetPatrolPointNextData(Data.Index, Data.SectionId, ChosenData))
			{
				TargetPatrolPointIndex = ChosenData.Index;
			}
		}
		else
		{
			if (TrackedPatrolPathManager->TryGetPatrolPointPriorData(Data.Index, Data.SectionId, ChosenData))
			{
				TargetPatrolPointIndex = ChosenData.Index;
			}
		}
	}
}

// Called every frame
void UPatrolTrackerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UPatrolTrackerComponent::SetTrackedPatrolSection(AAIPointContextManager* Manager, int32 SectionID)
{
	if (Manager)
	{
		FPatrolPointData ClosestData;
		if (Manager->TryGetClosestPatrolPointDataFromSection(SectionID, GetOwner()->GetActorLocation(), ClosestData))
		{
			TrackedPatrolPathManager = Manager;
			TargetPatrolPointIndex = ClosestData.Index;
			TrackedPatrolSection = ClosestData.SectionId;
			EntryPointIndex = TargetPatrolPointIndex;
			return true;
		}
	}

	return false;
}

bool UPatrolTrackerComponent::SetTrackedPatrolPoint(AAIPointContextManager* Manager, int32 SectionID, int32 Index)
{
	if (Manager)
	{
		FPatrolPointData Data;
		if (Manager->TryGetPatrolPointData(SectionID, Index, Data))
		{
			TrackedPatrolPathManager = Manager;
			TargetPatrolPointIndex = Data.Index;
			TrackedPatrolSection = Data.SectionId;
			EntryPointIndex = TargetPatrolPointIndex;
			return true;
		}
	}

	return false;
}

// FVector UPatrolTrackerComponent::FollowClosestPatrolPath(AAIPointContextManager* Manager)
// {
// 	if (Manager)
// 	{
// 		FPatrolPointData ClosestData;
// 		if (Manager->TryGetClosestPatrolPointData(GetOwner()->GetActorLocation(), ClosestData))
// 		{
// 			TrackedPatrolPathManager = Manager;
// 			TargetPatrolPointIndex = ClosestData.Index;
// 			TrackedPatrolSection = ClosestData.SectionId;
// 			EntryPointIndex = TargetPatrolPointIndex;
// 			return ClosestData.Location;
// 		}
// 	}
// 
// 	return FVector(FLT_MAX);
// }

FVector UPatrolTrackerComponent::FollowClosestIndexInSection(AAIPointContextManager* Manager, int32 Section)
{
	if (Manager)
	{
		FPatrolPointData ClosestData;
		if (Manager->TryGetClosestPatrolPointDataFromSection(Section, GetOwner()->GetActorLocation(), ClosestData))
		{
			TargetPatrolPointIndex = ClosestData.Index;
			TrackedPatrolSection = ClosestData.SectionId;
			EntryPointIndex = TargetPatrolPointIndex;
			return ClosestData.Location;
		}
	}

	return FVector(FLT_MAX);
}

FVector UPatrolTrackerComponent::UpdateNextPatrolLocation()
{
	if (TrackedPatrolPathManager && TargetPatrolPointIndex > -1)
	{
		FPatrolPointData Data;
		if (TrackedPatrolPathManager->TryGetPatrolPointData(TargetPatrolPointIndex, TrackedPatrolSection, Data))
		{
			switch (TraversalMode)
			{
			case EPatrolTraversalMode::Loop:
				TraverseLoop(Data, false);
			case EPatrolTraversalMode::Loop_Reversed:
				TraverseLoop(Data, true);
			default:
				break;
			}

			return Data.Location;
		}
	}

	return FVector(FLT_MAX); //invalid vector
}

void UPatrolTrackerComponent::SetTraversalMode(EPatrolTraversalMode InMode)
{
	TraversalMode = InMode;
}

