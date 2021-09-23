
#pragma once

#include "Engine/TargetPoint.h"
#include "AIPointContextManager.generated.h"

// type of points to add
UENUM()
enum EPointType
{
	Search = 0,
	SoundTransfer = 1,
	Patrol = 2
};

USTRUCT()
struct GHOSTINTHEGRAVEYARD_API FPatrolPointData
{
	GENERATED_BODY()

	UPROPERTY()
	int32 SectionId = -1;

	UPROPERTY()
	int32 Index = -1;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	TArray<int32> LinkedPatrolIndex;

	UPROPERTY()
	TArray<int32> LinkedPatrolSection;
};

USTRUCT()
struct GHOSTINTHEGRAVEYARD_API FPatrolSection
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int32 SectionID = -1;

	UPROPERTY()
	TArray<FPatrolPointData> PatrolPoints;
};

UCLASS()
class GHOSTINTHEGRAVEYARD_API AAIPointContextManager : public ATargetPoint
{
	GENERATED_BODY()

public:

	AAIPointContextManager();

	UPROPERTY(EditAnywhere, Category = "Points")
	TArray<FVector> SearchPoints;

	UPROPERTY(EditAnywhere, Category = "Points")
	TArray<FVector> SoundTransferPoints;

private:

	UPROPERTY(EditAnywhere)
	TArray<FPatrolSection> PatrolSections;

public:

	/**
	 * Creates a new patrol section with the new patrol point.
	 * Returns the ID for the patrol section.
	 */
	UFUNCTION(BlueprintCallable)
	int32 CreatePatrolSection(FVector FirstPoint);

	/**
	 * Add a patrol point to a patrol section.
	 */
	UFUNCTION(BlueprintCallable)
	int32 AddPatrolPointToSection(FVector Point, int32 SectionID);

	/**
	 * Creates a link between two patrol points.
	 */
	UFUNCTION(BlueprintCallable)
	void LinkPatrolPoints(int32 FromPointIndex, int32 FromPointSection, int32 ToPointIndex, int32 ToPointSection, bool bLinkBothWays = false);

	/*
	* Get the number of patrol sections;
	*/
	UFUNCTION(BlueprintCallable)
	int32 GetPatrolSectionNum() const { return PatrolSections.Num(); }

	/**
	 * Checks to see if the section id and patrol point index is valid.
	 */
	UFUNCTION(BlueprintCallable)
	bool IsValid(int32 SectionID, int32 PointIndex) const;

	/*
	* Attempts to a reference to the patrol point data at a given index and section.
	*/
	UFUNCTION()
	bool TryGetPatrolPointData(int32 PointIndex, int32 SectionID, FPatrolPointData& Data) const;

	/**
	 * Get the number of patrol points in a patrol section.
	 */
	UFUNCTION()
	int32 GetPatrolSectionPatrolNum(int32 SectionID) const { return PatrolSections.IsValidIndex(SectionID) ? PatrolSections[SectionID].PatrolPoints.Num() : 0; }

	/**
	 * Get Patrol data for reading.
	 */
	const TArray<FPatrolPointData>* GetPatrolPointData(int32 SectionID) const;

	/**
	 * Get the ptr of the patrol point's location vector.
	 */
	FVector* GetPatrolPointVectorPtr(int32 SectionID, int32 PointIndex);

protected:
	virtual void BeginPlay() override;

	
};