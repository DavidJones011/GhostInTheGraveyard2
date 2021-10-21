// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTask_HitObstacle.h"
#include "Gizmos/NavModifier_BreakableObstacle.h"
#include "CreatureCharacter.h"
#include "CreatureAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_HitObstacle::UBTTask_HitObstacle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Hit Obstacle";

	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_HitObstacle, BlackboardKey), ANavModifier_BreakableObstacle::StaticClass());
}

EBTNodeResult::Type UBTTask_HitObstacle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACreatureAIController* CreatureController = Cast<ACreatureAIController>(OwnerComp.GetAIOwner());
	if (CreatureController)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		if (ensure(BlackboardComp))
		{
			ANavModifier_BreakableObstacle* Obstacle = Cast<ANavModifier_BreakableObstacle>(BlackboardComp->GetValueAsObject(BlackboardKey.SelectedKeyName));
			if (Obstacle != NULL)
			{				
				Obstacle->TakeDamage(Damage, FDamageEvent(), CreatureController, CreatureController->GetCharacter());
				if (Obstacle->IsBroken())
				{
					BlackboardComp->ClearValue(BlackboardKey.SelectedKeyName);
				}

				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
