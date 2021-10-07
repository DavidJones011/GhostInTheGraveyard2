// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIWorldContextSubsystem.h"
#include "CreatureAIController.h"
#include "AI/AIPointContextManager.h"
#include "GameFramework/Character.h"

UAIWorldContextSubsystem::UAIWorldContextSubsystem()
: Super()
, TrackedController(nullptr)
, TrackedManager(nullptr)
{

}

void UAIWorldContextSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UAIWorldContextSubsystem::RegisterAIContextManager(AAIPointContextManager* Manager)
{
	// only register the first object
	if(TrackedManager == nullptr)
		TrackedManager = Manager;
}

void UAIWorldContextSubsystem::RegisterAIController(ACreatureAIController* Controller)
{
	// only register the first object
	if (TrackedController == nullptr)
		TrackedController = Controller;
}

void UAIWorldContextSubsystem::SendAIToPatrolSection(int32 Section, bool bTeleport /*= false*/)
{
	ACreatureAIController* Controller = GetAIController();
	AAIPointContextManager* Manager = GetAIContextManager();

	if (Controller != nullptr && Manager != nullptr)
	{
		Controller->SendAIToPatrolSection(Manager, Section, bTeleport);
	}
}

void UAIWorldContextSubsystem::SendAIToPatrolPoint(int32 Section, int32 Index, bool bTeleport /*= false*/)
{
	ACreatureAIController* Controller = GetAIController();
	AAIPointContextManager* Manager = GetAIContextManager();

	if (Controller != nullptr && Manager != nullptr)
	{
		Controller->SendAIToPatrolPoint(Manager, Section, Index, bTeleport);
	}
}
