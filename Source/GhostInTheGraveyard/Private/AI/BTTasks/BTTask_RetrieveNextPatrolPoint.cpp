// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTTasks/BTTask_RetrieveNextPatrolPoint.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "VisualLogger/VisualLogger.h"
#include "AIController.h"
#include "CreatureAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_RetrieveNextPatrolPoint::UBTTask_RetrieveNextPatrolPoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Retrieve Next Patrol Point";

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RetrieveNextPatrolPoint, BlackboardKey));
}

EBTNodeResult::Type UBTTask_RetrieveNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACreatureAIController* CreatureController = Cast<ACreatureAIController>(OwnerComp.GetAIOwner());
	if (CreatureController)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		if (ensure(BlackboardComp))
		{
			FVector PatrolLocation = CreatureController->UpdateNextPatrolPoint();
			BlackboardComp->SetValueAsVector(BlackboardKey.SelectedKeyName, PatrolLocation);

			// invalid patrol location was found
			if (PatrolLocation == FAISystem::InvalidLocation)
			{
				return EBTNodeResult::Failed;
			}

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
