// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DetectorComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UDetectorComponent::UDetectorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UDetectorComponent::BeginPlay()
{
	Super::BeginPlay();
}

#if WITH_EDITOR
void UDetectorComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetFName());

		if (PropertyName == GET_MEMBER_NAME_CHECKED(UDetectorComponent, TimeNeededToBeCautious))
		{
			TimeNeededToBeCautious = FMath::Clamp<float>(TimeNeededToBeCautious, 0.0F, TimeNeededToBeCurious);
		}
		else if (PropertyName == GET_MEMBER_NAME_CHECKED(UDetectorComponent, TimeNeededToBeCurious))
		{
			TimeNeededToBeCurious = FMath::Clamp<float>(TimeNeededToBeCurious, TimeNeededToBeCautious, TimeNeededToBeAware);
		}
		else if (PropertyName == GET_MEMBER_NAME_CHECKED(UDetectorComponent, TimeNeededToBeAware))
		{
			TimeNeededToBeAware = FMath::Clamp<float>(TimeNeededToBeAware, 0.0001F, FLT_MAX);
			TimeNeededToBeCurious = FMath::Clamp<float>(TimeNeededToBeCurious, TimeNeededToBeCautious, TimeNeededToBeAware);
			TimeNeededToBeCautious = FMath::Clamp<float>(TimeNeededToBeCautious, 0.0F, TimeNeededToBeCurious);
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

// Called every frame
void UDetectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (AActor* Actor : ActiveDetectionSet)
	{
		FDetectionData& Data = DetectionMap[Actor];

		// skip update for aware detections if paused
		if(Data.Stage == EDetectionStage::Aware && bPauseAwareDetect)
			continue;

		if (Data.Status == EDetectionStatus::PendingRemoval)
		{
			// mark for removal
			continue;
		}

		int32 bDirection = 2 * (int32)(Data.Visibility > 0) - 1;
		if (bDirection > 0)
			Data.Time += (Data.Visibility * DetectionRate) * DeltaTime;
		else
			Data.Time -= LooseTargetRate * DeltaTime;

		Data.Time = FMath::Clamp<float>(Data.Time, 0.0F, TimeNeededToBeAware + LooseTargetDelay);

		EDetectionStage Stage = EDetectionStage::Unaware;
		if (Data.Time >= TimeNeededToBeAware)
		{
			Stage = EDetectionStage::Aware;
		}
		else if (Data.Time >= TimeNeededToBeCurious)
		{
			Stage = EDetectionStage::Curious;
		}
		else if (Data.Time >= TimeNeededToBeCautious)
		{
			Data.Stage = EDetectionStage::Cautious;
		}

		if (Stage != Data.Stage)
		{
			if(Stage == EDetectionStage::Aware)
				Data.Time = TimeNeededToBeAware + LooseTargetDelay;

			Data.Stage = Stage;
			OnStageChanged.ExecuteIfBound(Data.DetectedActor, (uint32)Data.Stage);
		}
	}

	for(AActor* Actor : PendingDetectionSet)
	{
		FDetectionData& Data = DetectionMap[Actor];
		Data.Status = EDetectionStatus::Active;
		ActiveDetectionSet.Add(Actor);
	}
	PendingDetectionSet.Reset();
}

void UDetectorComponent::SetDetectedActor(class AActor* DetectedActor, float Visibility)
{
	// exit out early if the detected actor isn't valid
	if (DetectedActor == nullptr)
		return;

	Visibility = FMath::Clamp<float>(Visibility, 0.0F, 1.0F);

	// Check if the detected actor exists
	if (DetectionMap.Num() > 0)
	{
		FDetectionData* Data = DetectionMap.Find(DetectedActor);
		if (Data != NULL)
		{
			Data->Visibility = Visibility;
			return;
		}
	}

	// Add it to the map
	FDetectionData NewData;
	NewData.DetectedActor = DetectedActor;
	NewData.Visibility = Visibility;
	NewData.Stage = EDetectionStage::None;
	NewData.Status = EDetectionStatus::Pending;

	PendingDetectionSet.Add(DetectedActor);
	DetectionMap.Add(DetectedActor, MoveTemp(NewData));
}

void UDetectorComponent::InstantlyDetectActor(class AActor* DetectedActor)
{
	// exit out early if the detected actor isn't valid
	if (DetectedActor == nullptr)
		return;

	SetDetectedActor(DetectedActor, 1.0F);

	FDetectionData* Data = DetectionMap.Find(DetectedActor);
	if (Data != NULL)
	{
		Data->Time = TimeNeededToBeAware + LooseTargetDelay;
	}
}

void UDetectorComponent::GetDetectedArray(TArray<FDetectionResult>& OutData)
{
	for (AActor* Actor : ActiveDetectionSet)
	{
		FDetectionData& Data = DetectionMap[Actor];
		if (Data.Status == EDetectionStatus::PendingRemoval)
		{
			continue;
		}
		FDetectionResult Result;
		Result.Value = FMath::Clamp<float>(Data.Time / TimeNeededToBeAware, 0.0F, 1.0F);
		Result.Stage = Data.Stage;
		Result.Actor = Actor;
		OutData.Push(Result);
	}
}

void UDetectorComponent::PauseAwareDetections()
{
	bPauseAwareDetect = true;
}

void UDetectorComponent::ResumeAwareDetections()
{
	bPauseAwareDetect = false;
}

