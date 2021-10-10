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

void UAIDirectorSubsystem::SendAIToPatrolSection(int32 Section)
{
	if(AIController == nullptr || PatrolManager == nullptr)
		return;

	UPatrolTrackerComponent* PatrolTrackerComp = AIController->GetPatrolTrackerComponent();
	if (PatrolTrackerComp)
	{
		int32 CurrentSection = PatrolTrackerComp->GetTrackedPatrolSection();
		if (CurrentSection != Section)
		{
			PatrolTrackerComp->SetTrackedPatrolSection(PatrolManager, Section);
		}
	}
}

void UAIDirectorSubsystem::TeleportAIToPatrolPoint(int32 Section, int32 Index)
{
	
}

