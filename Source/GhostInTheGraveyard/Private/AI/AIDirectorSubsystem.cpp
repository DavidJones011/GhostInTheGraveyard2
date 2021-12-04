// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIDirectorSubsystem.h"
#include "CreatureAIController.h"
#include "SurvivorCharacter.h"
#include "Components/PatrolTrackerComponent.h"
#include "AI/AIPointContextManager.h"
#include "Components/DetectorComponent.h"
#include "Components/DialogueComponent.h"
#include "Dialogue/DialogueActor.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Gizmos/CheckpointActor.h"
#include "Camera/CameraComponent.h"

UAIDirectorSubsystem::UAIDirectorSubsystem()
: Super()
, PlayerCharacter(nullptr)
, AIController(nullptr)
, PatrolManager(nullptr)
{

}

void UAIDirectorSubsystem::RecordState(ACheckpointActor* CheckpointActor)
{
	if (LastRecord.LastCheckpoint.IsValid())
	{
		LastRecord.LastCheckpoint->ChangeCheckPointState(ECheckpointState::CPS_VISITED);
	}

	if (PlayerCharacter)
	{
		LastRecord.PlayerLocation = PlayerCharacter->GetActorLocation();
		LastRecord.PlayerRotation = PlayerCharacter->GetFirstPersonCameraComponent()->GetComponentRotation();
	}
	else
	{
		LastRecord.PlayerLocation = FVector::ZeroVector;
		LastRecord.PlayerRotation = FRotator::ZeroRotator;
	}

	if (AIController && AIController->GetCharacter() && AIController->GetPatrolTrackerComponent())
	{
		LastRecord.AISection = AIController->GetPatrolTrackerComponent()->GetTrackedPatrolSection();
		LastRecord.AICharacterLocation = AIController->GetNavAgentLocation();
	}
	else
	{
		LastRecord.AISection = -1;
		LastRecord.AICharacterLocation = FVector::ZeroVector;
	}

	LastRecord.LastCheckpoint = CheckpointActor;
	CheckpointActor->ChangeCheckPointState(ECheckpointState::CPS_ACTIVE);
}

void UAIDirectorSubsystem::LoadRecordedState()
{
	if (AIController)
	{
		SetAIForgetPlayer();
		ACharacter* AICharacter = AIController->GetCharacter();
		if (AICharacter) { AICharacter->TeleportTo(LastRecord.AICharacterLocation, FRotator::ZeroRotator); }	
		PlaceAIAtPatrolPoint(LastRecord.AISection, -1);
	}

	if (PlayerCharacter)
	{
		PlayerCharacter->TeleportTo(LastRecord.PlayerLocation, LastRecord.PlayerRotation);
		PlayerCharacter->Hidden = false;

		if (PlayerCharacter->GetController() && PlayerCharacter->GetFirstPersonCameraComponent())
		{
			PlayerCharacter->GetController()->SetIgnoreMoveInput(false);
			PlayerCharacter->GetController()->SetControlRotation(LastRecord.PlayerRotation);
		}
	}
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

void UAIDirectorSubsystem::SetAIForgetPlayer()
{
	if (PlayerCharacter && AIController) 
	{
		if (AIController->GetDetectorComponent())
		{
			AIController->GetDetectorComponent()->InstantlyLooseActor(PlayerCharacter);
		}
	}
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

	if (AIController->CanBeTeleported())
	{
		ACharacter* AICharacter = AIController->GetCharacter();

		if (!AICharacter)
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


			UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
			AICharacter->SetActorLocation(NavSys->ProjectPointToNavigation(GetWorld(), Data.Location));
		}
	}
}

