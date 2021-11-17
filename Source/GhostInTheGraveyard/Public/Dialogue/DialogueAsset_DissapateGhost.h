// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue/DialogueAsset_Execute.h"
#include "DialogueAsset_DissapateGhost.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API UDialogueAsset_DissapateGhost : public UDialogueAsset_Execute
{
	GENERATED_BODY()

public:
	virtual void Execute(AActor* ThisActor, AActor* Instigator);
};
