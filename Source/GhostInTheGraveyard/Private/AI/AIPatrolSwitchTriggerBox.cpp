// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPatrolSwitchTriggerBox.h"
#include "Components/ShapeComponent.h"
#include "AI/AIDirectorSubsystem.h"

AAIPatrolSwitchTriggerBox::AAIPatrolSwitchTriggerBox()
: Super()
{
	UShapeComponent* Shape = GetCollisionComponent();

	if (Shape)
	{
		Shape->SetGenerateOverlapEvents(true);
		Shape->OnComponentBeginOverlap.AddDynamic(this, &AAIPatrolSwitchTriggerBox::BeginOverlap);
		//Shape->OnComponentEndOverlap.AddDynamic(this, &AAIPatrolSwitchTriggerBox::EndOverlap);
	}
}

void AAIPatrolSwitchTriggerBox::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UWorld* World = GetWorld();
	if (World)
	{
		Switch();
	}
}

void AAIPatrolSwitchTriggerBox::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AAIPatrolSwitchTriggerBox::Switch()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UAIDirectorSubsystem* Director = World->GetSubsystem<UAIDirectorSubsystem>();
		if (Director && PatrolSection > -1)
		{
// 			if (PatrolPointIndex > -1)
// 			{
// 				Director->SendAIToPatrolPoint(PatrolSection, PatrolPointIndex, bTeleport);
// 			}
//		else
//		{
				Director->SendAIToPatrolSection(PatrolSection);
//			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("The patrol section was NOT valid."));
		}
	}
}
