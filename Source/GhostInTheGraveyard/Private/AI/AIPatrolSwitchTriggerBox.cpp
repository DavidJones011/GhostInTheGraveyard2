// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPatrolSwitchTriggerBox.h"
#include "Components/ShapeComponent.h"
#include "AI/AIDirectorSubsystem.h"
#include "GameFramework/Character.h"

AAIPatrolSwitchTriggerBox::AAIPatrolSwitchTriggerBox()
: Super()
{
	UShapeComponent* Shape = GetCollisionComponent();

	if (Shape)
	{
		Shape->SetGenerateOverlapEvents(true);
		Shape->OnComponentBeginOverlap.AddDynamic(this, &AAIPatrolSwitchTriggerBox::BeginOverlap);
		Shape->OnComponentEndOverlap.AddDynamic(this, &AAIPatrolSwitchTriggerBox::EndOverlap);
	}
}

void AAIPatrolSwitchTriggerBox::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(bOnExit)
		return;

	UWorld* World = GetWorld();
	if (World)
	{
		ACharacter* Character =  Cast<ACharacter>(OtherActor);
		if (Character->IsPlayerControlled())
		{
			Switch();
		}
	}
}

void AAIPatrolSwitchTriggerBox::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!bOnExit)
		return;

	UWorld* World = GetWorld();
	if (World)
	{
		ACharacter* Character = Cast<ACharacter>(OtherActor);
		if (Character && Character->IsPlayerControlled())
		{
			Switch();
		}
	}
}

void AAIPatrolSwitchTriggerBox::Switch()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UAIDirectorSubsystem* Director = World->GetSubsystem<UAIDirectorSubsystem>();
		if (Director)
		{
			if (!bDoNotAssignPatrolRoute)
			{
				if (PatrolPointIndex > -1)
				{
					Director->SendAIToPatrolPoint(PatrolSection, PatrolPointIndex, Delay, TraverseMode, bTeleport);
				}
				else
				{
					Director->SendAIToPatrolSection(PatrolSection, Delay, TraverseMode, bTeleport);
				}
			}

			if (bTeleport)
			{
				if (bDoNotAssignPatrolRoute)
				{
					Director->PlaceAIAtPatrolPoint(PatrolSection, PatrolPointIndex);
				}

				switch (TeleportState)
				{
				case ETeleportState::TS_Investigate:
					Director->SendAIToInvestigatePlayerLocation();
					break;
				case ETeleportState::TS_Pursue:
					Director->SetAIAwareOfPlayer();
					break;
				default:
					break;
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("The patrol section was NOT valid."));
		}
	}

	if (bDisableAfterOnce)
	{
		UShapeComponent* Shape = GetCollisionComponent();
		Shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
