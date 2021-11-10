// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue/DialogueAsset.h"
#include "DialogueAsset_Ordered.generated.h"

class USoundBase;

USTRUCT()
struct GHOSTINTHEGRAVEYARD_API FDialogueLine
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float DialogueDuration = 0.0;

	UPROPERTY(EditAnywhere)
	bool bUseAudioTime = true;
	 
	UPROPERTY(EditAnywhere)
	USoundBase* Audio = nullptr;

	UPROPERTY(EditAnywhere)
	FText Text;
};


/**
 * 
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API UDialogueAsset_Ordered : public UDialogueAsset
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere)
	UDialogueAsset* NextDialogue = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<FDialogueLine> Lines;

	UPROPERTY(Transient)
	int32 CurrentIndex = -1;

public:

	/* UDialogueAsset interface */
	virtual USoundBase* GetSound() const override { return Lines.IsValidIndex(CurrentIndex) ? Lines[CurrentIndex].Audio : nullptr; }
	virtual float GetTimeDuration() const override;
	virtual FText GetDialogueText() const override { return Lines.IsValidIndex(CurrentIndex) ? Lines[CurrentIndex].Text : FText::FromString(TEXT("No line exists.")); }
	virtual UDialogueAsset* GetNextDialogue() override;
	virtual bool SendInput(FName Input) override { return true; }
	virtual void OnDialogueEnter() override { CurrentIndex = 0; }
	/* UDialogueAsset interface */
};
