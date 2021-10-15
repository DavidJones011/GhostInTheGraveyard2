// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Components/PatrolTrackerComponent.h"
#include "AIPatrolSwitchTriggerBox.generated.h"

UENUM(BlueprintType)
enum ETeleportState
{
	TS_Investigate = 0  UMETA(DisplayName = "Investigate"),
	TS_Pursue      = 1  UMETA(DisplayName = "Pursue"),
	TS_NoChange    = 2  UMETA(DisplayName = "No Change")
};

/**
 * 
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API AAIPatrolSwitchTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:

	AAIPatrolSwitchTriggerBox();

protected:

	// Patrol section to send the AI.
	UPROPERTY(EditAnywhere)
	int32 PatrolSection = 0;

	// Teleport the AI to the given index of the patrol section.
	// If -1, then the closest point in the section will be selected.
	UPROPERTY(EditAnywhere)
	int32 PatrolPointIndex = -1;
	
	// The patrol traversal mode.
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EPatrolTraversalMode> TraverseMode;

	// Teleport the AI to the given section and point index index.
	UPROPERTY(EditAnywhere)
	bool bTeleport = false;

	// Delay the signal to tell the AI to switch patrol sections and/or points.
	UPROPERTY(EditAnywhere)
	float Delay = 0.0F;

	// Disables the trigger after the first overlap collision response.
	UPROPERTY(EditAnywhere)
	bool bDisableAfterOnce = false;

	// Send data to the director on exiting the trigger. If false, data is sent when entering the trigger.
	UPROPERTY(EditAnywhere)
	bool bOnExit = false;

	// Does not allow for the patrol section to be set. Only used when teleporting.
	UPROPERTY(EditAnywhere)
	bool bDoNotAssignPatrolRoute = false;

	// The state the AI should be in when teleported. Only used when teleporting.
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ETeleportState> TeleportState = ETeleportState::TS_NoChange;

protected:

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void Switch();
};
