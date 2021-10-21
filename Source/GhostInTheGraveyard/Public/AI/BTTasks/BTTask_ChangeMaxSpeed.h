// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ChangeMaxSpeed.generated.h"

struct FBTChangeMaxSpeedMemory
{
	float CurrentTime;
	float StepDeltaTime;
	int32 Step;
};

/**
 * 
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API UBTTask_ChangeMaxSpeed : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()

	// The number of steps to increase the max speed
	UPROPERTY(Category = ChangeMaxSpeed, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	int32 SpeedSteps;

	// The starting max speed
	UPROPERTY(Category = ChangeMaxSpeed, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float StartSpeed;

	// The max speed goal
	UPROPERTY(Category = ChangeMaxSpeed, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float EndSpeed;

	// Duration to accelerate to the end max speed from the start max speed
	UPROPERTY(Category = ChangeMaxSpeed, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float Duration;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual uint16 GetInstanceMemorySize() const override;
	
protected:
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
