// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DetectorComponent.generated.h"

// Status used to manage the data in the detector component
enum class EDetectionStatus
{
	None,
	Pending,
	Active,
	PendingRemoval
};

// Different stages of the enemy detecting something
enum class EDetectionStage
{
	Unaware  = 0,
	Cautious = 1,
	Curious  = 2,
	Aware    = 4
};

// Detection Data
struct FDetectionData
{
	class AActor* DetectedActor;
	float Time;
	float Visibility;
	float DetectionRatio;
	EDetectionStage Stage;
	EDetectionStatus Status;

	FDetectionData()
		: DetectedActor(nullptr)
		, Time(0.0F)
		, Visibility(0.0F)
		, DetectionRatio(0.0F)
		, Stage(EDetectionStage::Unaware)
		, Status(EDetectionStatus::None)
	{
	}

	// Movable only
	FDetectionData(FDetectionData&&) = default;
	FDetectionData(const FDetectionData&) = delete;
	FDetectionData& operator=(FDetectionData&&) = default;
	FDetectionData& operator=(const FDetectionData&) = delete;
};

// Used to get useful information outside
USTRUCT()
struct FDetectionResult
{
	GENERATED_USTRUCT_BODY()

	float Value;
	AActor* Actor;
	EDetectionStage Stage;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GHOSTINTHEGRAVEYARD_API UDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDetectorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Sets the actor to be detected with a certain visibility (0 to 1).
	 * If the actor doesn't exist in the list of detections it is added.
	 */
	UFUNCTION(BlueprintCallable)
	void SetDetectedActor(class AActor* DetectedActor, float Visibliity);

	/**
	 * Returns the array of data of all actors that are considered detected.
	 * Stage param allows to filter the results.
	 */
	void GetDetectedArray(TArray<FDetectionResult>& OutResults, EDetectionStage Stage);

	/**
	 * Delegate that is broadcasted when the detecting stage of an actor has changed.
	 */
	DECLARE_DELEGATE_TwoParams(FOnStageChanged, AActor*, uint32);
	FOnStageChanged OnStageChanged;

protected:

	/* Time needed to be considered aware of a detected object.*/
	UPROPERTY(EditAnywhere, Category = Detection)
	float TimeNeededToBeAware = 1.0F;

	UPROPERTY(EditAnywhere, Category = Detection)
	float TimeNeededToBeCautious = 0.7F;

	UPROPERTY(EditAnywhere, Category = Detection)
	float TimeNeededToBeCurious = 0.5F;

	UPROPERTY(EditAnywhere, Category = Detection)
	float LooseTargetDelay = 1.0F;

private:

	/* The rate to detect objects.*/
	UPROPERTY(EditAnywhere, Category = Detection)
	float DetectionRate = 1.0F;

	/* The rate to undetect objects.*/
	UPROPERTY(EditAnywhere, Category = Detection)
	float LooseTargetRate = 2.0F;

	TMap<AActor*, FDetectionData> DetectionMap;
	TSet<AActor*> ActiveDetectionSet;
	TSet<AActor*> PendingDetectionSet;
};