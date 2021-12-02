// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue/DialogueAsset_RunMissionEvent.h"
#include "Dialogue/MissionActor.h"

void UDialogueAsset_RunMissionEvent::Execute(AActor* ThisActor, AActor* Instigator)
{
	AMissionActor* MissionActor = Cast<AMissionActor>(ThisActor);
	if (MissionActor)
	{
		MissionActor->CustomMissionEvent();
	}
}
