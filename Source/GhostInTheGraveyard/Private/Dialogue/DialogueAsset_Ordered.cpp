// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue/DialogueAsset_Ordered.h"
#include "Sound/SoundBase.h"

float UDialogueAsset_Ordered::GetTimeDuration() const
{
	if (Lines.IsValidIndex(CurrentIndex))
	{
		return (Lines[CurrentIndex].Audio && Lines[CurrentIndex].bUseAudioTime) ? Lines[CurrentIndex].Audio->GetDuration() : Lines[CurrentIndex].DialogueDuration;
	}

	return 0.0F;
}

UDialogueAsset* UDialogueAsset_Ordered::GetNextDialogue()
{
	CurrentIndex++;
	return (CurrentIndex >= Lines.Num()) ? NextDialogue : this;
}
