// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue/DialogueAsset_Execute.h"
#include "DialogueAsset_RunMissionEvent.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API UDialogueAsset_RunMissionEvent : public UDialogueAsset_Execute
{
	GENERATED_BODY()

public:
	virtual void Execute(AActor* ThisActor, AActor* Instigator);
};
