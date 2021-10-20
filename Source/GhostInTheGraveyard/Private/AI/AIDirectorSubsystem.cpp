// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIDirectorSubsystem.h"
#include "CreatureAIController.h"
#include "SurvivorCharacter.h"
#include "Components/PatrolTrackerComponent.h"
#include "AI/AIPointContextManager.h"

UAIDirectorSubsystem::UAIDirectorSubsystem()
: Super()
, PlayerCharacter(nullptr)
, AIController(nullptr)
, PatrolManager(nullptr)
{

}

void UAIDirectorSubsystem::RegisterPlayer(ASurvivorCharacter* Character)
{
	if (Character != nullptr && !Character->IsPendingKill())
	{
		PlayerCharacter = Character;
	}
}

void UAIDirectorSubsystem::UnregisterPlayer()
{
	PlayerCharacter = nullptr;
}

void UAIDirectorSubsystem::RegisterAIController(ACreatureAIController* Controller)
{
	if (Controller != nullptr && !Controller->IsPendingKill())
	{
		AIController = Controller;
	}
}

void UAIDirectorSubsystem::UnregisterAIController()
{
	AIController = nullptr;
}

void UAIDirectorSubsystem::RegisterAIContextManager(AAIPointContextManager* Manager)
{
	if (Manager != nullptr && !Manager->IsPendingKill())
	{
		PatrolManager = Manager;
	}
}

void UAIDirectorSubsystem::UnregisterAIContextManager()
{
	PatrolManager = nullptr;
}

void UAIDirectorSubsystem::SetAIAwareOfActor(AActor* DetectedActor)
{
	if (AIController == nullptr)
		return;

	AIController->InstantlyDetect(DetectedActor);
}

void UAIDirectorSubsystem::SetAIAwareOfPlayer()
{
	if (PlayerCharacter) { SetAIAwareOfActor(PlayerCharacter); }
}

void UAIDirectorSubsystem::SendAIToInvestigateLocation(FVector Location)
{
	if (AIController == nullptr && Location != FVector(FLT_MAX))
		return;

	AIController->InvestigateLocation(Location);
}

void UAIDirectorSubsystem::SendAIToInvestigatePlayerLocation()
{
	if (PlayerCharacter) { SendAIToInvestigateLocation(PlayerCharacter->GetActorLocation()); }
}

void UAIDirectorSubsystem::SendAIToPatrolPoint_Impl(int32 Section, int32 Index, EPatrolTraversalMode TraverseMode, bool bTeleport)
{
	if (AIController == nullptr || PatrolManager == nullptr)
		return;

	if (Index < 0)
		AIController->SendAIToPatrolSection(PatrolManager, Section, bTeleport);
	else
		AIController->SendAIToPatrolPoint(PatrolManager, Section, Index, bTeleport);
}

void UAIDirectorSubsystem::SendAIToPatrolPoint(int32 Section, int32 Index, float Delay /*= 0.0F*/, EPatrolTraversalMode TraverseMode /*= EPatrolTraversalMode::Loop*/, bool bTeleport /*= false*/)
{
	if (FMath::IsNearlyZero(Delay))
	{
		SendAIToPatrolPoint_Impl(Section, Index, TraverseMode, bTeleport);
	}
	else
	{	
		UWorld* World = GetWorld();
		if (World)
		{
			FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &UAIDirectorSubsystem::SendAIToPatrolPoint_Impl, Section, Index, TraverseMode, bTeleport);
			World->GetTimerManager().SetTimer(TimerHandle, TimerDel, Delay, false);
		}
	}
}

void UAIDirectorSubsystem::PlaceAIAtPatrolPoint(int32 Section, int32 Index)
{
	if(!AIController)
		return;

	ACharacter* AICharacter = AIController->GetCharacter();

	if(!AICharacter)
		return;

	if (PatrolManager)
	{
		FPatrolPointData Data;
		if (Index > -1)
		{
			PatrolManager->TryGetPatrolPointData(Index, Section, Data);
		}
		else
		{
			PatrolManager->TryGetClosestPatrolPointDataFromSection(Section, AICharacter->GetActorLocation(), Data);
		}

		AICharacter->SetActorLocation(Data.Location);
	}
}

