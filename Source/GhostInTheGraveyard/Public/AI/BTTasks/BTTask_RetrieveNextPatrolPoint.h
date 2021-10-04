// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RetrieveNextPatrolPoint.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class GHOSTINTHEGRAVEYARD_API UBTTask_RetrieveNextPatrolPoint : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
