// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolTrackerComponent.generated.h"

class AAIPointContextManager;
struct FPatrolPointData;

UENUM()
enum EPatrolTraversalMode
{
	Loop           UMETA(DisplayName="Loop"),
	Loop_Reversed  UMETA(DisplayName = "Reversed Loop"),
	Ping_Pong	   UMETA(DisplayName = "Ping Pong"),
	Random         UMETA(DisplayName = "Random"),
	None           UMETA(DisplayName = "None (Off)")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GHOSTINTHEGRAVEYARD_API UPatrolTrackerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPatrolTrackerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FVector TraverseLoop(const FPatrolPointData& Data, bool bReverse = false);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FVector FollowClosestPatrolPath(AAIPointContextManager* Manager);

	/*
	* Update to the next patrol location based on the current traversal mode.
	*/
	UFUNCTION(BlueprintCallable)
	FVector UpdateNextPatrolLocation();

	/**
	 * Set the current patrol path traversal mode.
	 */
	UFUNCTION(BlueprintCallable)
	void SetTraversalMode(EPatrolTraversalMode InMode);

	/**
	 * Get the current patrol section the owner is following.
	 * Setting the traversal node to none, will stop following the patrol path.
	 */
	UFUNCTION(BlueprintCallable)
	int32 GetTrackedPatrolSection() const { return TrackedPatrolSection; }

	/**
	 * Get the current traversal mode.
	 */
	UFUNCTION(BlueprintCallable)
	EPatrolTraversalMode GetTraversalMode() const { return TraversalMode; }

private:

	/* The default patrol path traversal mode. */
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EPatrolTraversalMode> DefaultMode = EPatrolTraversalMode::Loop;

	/* The current patrol path traversal mode. */
	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<EPatrolTraversalMode> TraversalMode = EPatrolTraversalMode::None;

	/* The tracked patrol path manager. */
	UPROPERTY(VisibleAnywhere)
	AAIPointContextManager* TrackedPatrolPathManager = nullptr;

	/* The current tracked patrol section. */
	UPROPERTY(VisibleAnywhere)
	int32 TrackedPatrolSection = -1;

	/* The current patrol point index target. */
	UPROPERTY(VisibleAnywhere)
	int32 TargetPatrolPointIndex = -1;

	/* The patrol point index in which patrolling had started.*/
	UPROPERTY(VisibleAnywhere)
	int32 EntryPointIndex = -1;
};
