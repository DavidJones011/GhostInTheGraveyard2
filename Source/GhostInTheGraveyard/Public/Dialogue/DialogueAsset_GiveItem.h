// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue/DialogueAsset_Execute.h"
#include "DialogueAsset_GiveItem.generated.h"

/**
 *
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API UDialogueAsset_GiveItem : public UDialogueAsset_Execute
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	FName ItemName = "ItemName";

	UPROPERTY(EditAnywhere)
	USoundBase* ItemObtainedSound = nullptr;

public:
	virtual void Execute(AActor* ThisActor, AActor* Instigator);
};
