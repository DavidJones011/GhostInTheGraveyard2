// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue/DialogueAsset_Branch.h"

UDialogueAsset* UDialogueAsset_Branch::GetNextDialogue()
{
	if(BranchToTake == -1)
		return nullptr;

	return (Branches.IsValidIndex(BranchToTake)) ? Branches[BranchToTake].DialogueAsset : nullptr;
}

bool UDialogueAsset_Branch::SendInput(FName Input)
{
	int32 Index = 0;
	for (const FDialogueBranch& Branch : Branches)
	{
		if (Branch.NeededInput == Input)
		{
			BranchToTake = Index;
			return true;
		}
		Index++;
	}

	return false;
}
