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

	for (TActorIterator<AAIPointContextManager> It(GetWorld()); It; ++It)
	{
		AAIPointContextManager* Manager = (*It);
		if (Manager)
		{
			TrackedPatrolPathManager = Manager;
			break;
		}
	}

	FollowClosestPatrolPath(TrackedPatrolPathManager);
}

FVector UPatrolTrackerComponent::TraverseLoop(const FPatrolPointData& Data, bool bReverse /* = false*/)
{
	if (TrackedPatrolPathManager)
	{
		FPatrolPointData ChosenData;
		if (bReverse)
		{
			if (TrackedPatrolPathManager->TryGetPatrolPointNextData(Data.Index, Data.SectionId, ChosenData))
			{
				TargetPatrolPointIndex = ChosenData.Index;
				return ChosenData.Location;
			}
		}
		else
		{
			if (TrackedPatrolPathManager->TryGetPatrolPointPriorData(Data.Index, Data.SectionId, ChosenData))
			{
				TargetPatrolPointIndex = ChosenData.Index;
				return ChosenData.Location;
			}
		}
	}

	return FVector::ZeroVector;
}

// Called every frame
void UPatrolTrackerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FVector UPatrolTrackerComponent::FollowClosestPatrolPath(AAIPointContextManager* Manager)
{
	if (Manager)
	{
		FPatrolPointData ClosestData;
		if (Manager->TryGetClosestPatrolPointData(GetOwner()->GetActorLocation(), ClosestData))
		{
			TargetPatrolPointIndex = ClosestData.Index;
			TrackedPatrolSection = ClosestData.SectionId;
			EntryPointIndex = TargetPatrolPointIndex;
			return ClosestData.Location;
		}
	}

	return FVector::ZeroVector;
}

FVector UPatrolTrackerComponent::UpdateNextPatrolLocation()
{
	if (TrackedPatrolPathManager)
	{
		//get the entry point
		if (TargetPatrolPointIndex == -1)
		{
			FPatrolPointData Data;
			if (TrackedPatrolPathManager->TryGetPatrolPointData(0, TrackedPatrolSection, Data))
			{
				TargetPatrolPointIndex = Data.Index;
				return Data.Location;
			}
		}
		else
		{
			FPatrolPointData Data;
			if (TrackedPatrolPathManager->TryGetPatrolPointData(TargetPatrolPointIndex, TrackedPatrolSection, Data))
			{
				switch (TraversalMode)
				{
				case EPatrolTraversalMode::Loop:
					return TraverseLoop(Data, false);
				case EPatrolTraversalMode::Loop_Reversed:
					return TraverseLoop(Data, true);
				default:
					break;
				}
			}
		}
	}

	return FVector(FLT_MAX); //invalid vector
}

void UPatrolTrackerComponent::SetTraversalMode(EPatrolTraversalMode InMode)
{
	TraversalMode = InMode;
}

