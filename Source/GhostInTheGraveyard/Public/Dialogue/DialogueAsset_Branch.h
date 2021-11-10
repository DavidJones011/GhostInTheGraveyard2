// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue/DialogueAsset.h"
#include "DialogueAsset_Branch.generated.h"

USTRUCT()
struct GHOSTINTHEGRAVEYARD_API FDialogueBranch
{
	GENERATED_BODY()

	// Dialgoue Asset to branch off to
	UPROPERTY(EditAnywhere)
	UDialogueAsset* DialogueAsset;

	UPROPERTY(EditAnywhere)
	FName NeededInput = NAME_None;
};

/**
 * 
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API UDialogueAsset_Branch : public UDialogueAsset
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	TArray<FDialogueBranch> Branches;

	UPROPERTY(EditAnywhere)
	USoundBase* Sound = nullptr;

	UPROPERTY(EditAnywhere)
	FText Text;

	UPROPERTY(Transient)
	int32 BranchToTake = -1;

public:

	/* UDialogueAsset interface */
	virtual USoundBase* GetSound() const override { return Sound; }
	virtual float GetTimeDuration() const override { return -1.0F; }
	virtual FText GetDialogueText() const override { return Text; }
	virtual UDialogueAsset* GetNextDialogue() override;
	virtual bool SendInput(FName Input);
	virtual void OnDialogueEnter() override { BranchToTake = -1; }
	/* UDialogueAsset interface */
};
