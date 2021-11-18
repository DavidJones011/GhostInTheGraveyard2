// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue/DialogueActor.h"
#include "GhostActor.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API AGhostActor : public ADialogueActor
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	void Dissapate();

	virtual void Dissapate_Implementation() {};
};
