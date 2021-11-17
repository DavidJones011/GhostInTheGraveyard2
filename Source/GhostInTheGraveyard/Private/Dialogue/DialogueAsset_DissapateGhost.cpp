// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue/DialogueAsset_DissapateGhost.h"
#include "Dialogue/GhostActor.h"

void UDialogueAsset_DissapateGhost::Execute(AActor* ThisActor, AActor* Instigator)
{
	AGhostActor* GhostActor = Cast<AGhostActor>(ThisActor);
	if (GhostActor)
	{
		GhostActor->Dissapate();
	}
}
