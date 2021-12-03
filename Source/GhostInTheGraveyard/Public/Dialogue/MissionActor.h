// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue/DialogueActor.h"
#include "MissionActor.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API AMissionActor : public ADialogueActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	bool bCanInteract = true;

public:

	UFUNCTION(BlueprintNativeEvent)
	void CustomMissionEvent();
	virtual void CustomMissionEvent_Implementation(){};

	virtual bool CanInteract(ASurvivorCharacter* player) override { return bCanInteract; }

	UFUNCTION(BlueprintCallable)
	void SetCanInteract(bool Val) { bCanInteract = Val;}
	
};
