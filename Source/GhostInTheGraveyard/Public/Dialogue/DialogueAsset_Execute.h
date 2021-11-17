// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue/DialogueAsset.h"
#include "DialogueAsset_Execute.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API UDialogueAsset_Execute : public UDialogueAsset
{
	GENERATED_BODY()

public:
	UDialogueAsset_Execute() { WidgetName = "Execute"; }

protected:

	UPROPERTY(EditAnywhere)
	UDialogueAsset* NextDialogue = nullptr;

	UPROPERTY(EditAnywhere)
	float Duration;

public:

	/* UDialogueAsset interface */
	virtual USoundBase* GetSound() const override { return nullptr; }
	virtual float GetTimeDuration() const override { return Duration; }
	virtual FText GetDialogueText() const override { return FText(); }
	virtual UDialogueAsset* GetNextDialogue() override { return NextDialogue; }
	/* UDialogueAsset interface */

	virtual void Execute(AActor* ThisActor, AActor* Instigator) {};
};
