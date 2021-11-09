// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTTasks/BTTask_KillPlayer.h"
#include "SurvivorCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/DetectorComponent.h"
#include "CreatureAIController.h"

UBTTask_KillPlayer::UBTTask_KillPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Kill Player";

	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_KillPlayer, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_KillPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACreatureAIController* CreatureController = Cast<ACreatureAIController>(OwnerComp.GetAIOwner());
	if (CreatureController)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		if (ensure(BlackboardComp))
		{
			ASurvivorCharacter* PlayerCharacter = Cast<ASurvivorCharacter>(BlackboardComp->GetValueAsObject(BlackboardKey.SelectedKeyName));
			if (PlayerCharacter != NULL)
			{
				PlayerCharacter->Kill();
				CreatureController->GetDetectorComponent()->InstantlyLooseActor(PlayerCharacter);
				BlackboardComp->ClearValue(BlackboardKey.SelectedKeyName);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}