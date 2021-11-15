// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DialogueComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Dialogue/DialogueAsset.h"
#include "Dialogue/DialogueAsset_Branch.h"
#include "SurvivorCharacter.h"
#include "Components/InventoryComponent.h"
#include "Dialogue/DialogueUserWidget.h"

// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UDialogueComponent::StartConversation(ASurvivorCharacter* Instigator)
{
	if(Instigator == nullptr || Instigator->GetDialogueWidget() == nullptr)
		return;

	CurrentDialogueAsset = nullptr;
	if (StartingDialogueAsset)
	{
		InstigatingCharacter = Instigator;
		bConversationRunning = true;
		InstigatingCharacter->GetDialogueWidget()->BeginDialogue();
		RunDialogueAsset(StartingDialogueAsset);
	}
}

void UDialogueComponent::RunDialogueAsset(UDialogueAsset* Dialogue)
{
	// update the current dialogue asset
	if (Dialogue)
	{
		if (CurrentDialogueAsset != Dialogue)
		{
			if(CurrentDialogueAsset) CurrentDialogueAsset->OnDialogueExit();
			Dialogue->OnDialogueEnter();
			CurrentDialogueAsset = Dialogue;
		}
	}
	else
	{
		// no valid dialogue asset was found, exit the conversation
		if (CurrentDialogueAsset) CurrentDialogueAsset->OnDialogueExit();
		ExitConversation();
		return;
	}

	// run the dialogue asset
	if (CurrentDialogueAsset)
	{
		USoundBase* DialogueSound = CurrentDialogueAsset->GetSound();
		float DialogueDuration = CurrentDialogueAsset->GetTimeDuration();
		FText DialogueText = CurrentDialogueAsset->GetDialogueText();
		FName WidgetName = CurrentDialogueAsset->GetWidgetName();

		ensure(InstigatingCharacter && InstigatingCharacter->GetDialogueWidget());
		InstigatingCharacter->GetDialogueWidget()->SetDialogueWidget(WidgetName);
		InstigatingCharacter->GetDialogueWidget()->SetDialogueText(DialogueText);

		// checks for inventory item and sends it as input
		if (WidgetName == "ItemCheck" && InstigatingCharacter->GetInventoryComponent())
		{
			UDialogueAsset_Branch* Branch = Cast<UDialogueAsset_Branch>(CurrentDialogueAsset);
			if (Branch)
			{
				bool bFoundItem = false;
				for (FName RequiredInput : Branch->GetWantedInputs())
				{
					if (InstigatingCharacter->GetInventoryComponent()->RemoveItem(RequiredInput) > 0)
					{
						bFoundItem = true;
						SendInput(RequiredInput);
						break;
					}
				}

				if(!bFoundItem)
					SendInput();
			}
		}

		if (DialogueDuration >= 0.0)
		{
			if (DialogueSound && !FMath::IsNearlyZero(DialogueDuration)) UGameplayStatics::PlaySound2D(GetWorld(), DialogueSound);
			if (GetWorld())
			{
				GetWorld()->GetTimerManager().SetTimer(DialogueTimerHandle, this, &UDialogueComponent::DialogueTimerCallback, DialogueDuration);
			}
		}
		else
		{
			if (DialogueSound) UGameplayStatics::PlaySound2D(GetWorld(), DialogueSound);
			// no timer, awaits input for continuing
		}
	}
}

void UDialogueComponent::ExitConversation()
{
	/* update the starting dialogue asset */
	if (CurrentDialogueAsset)
	{
		StartingDialogueAsset = CurrentDialogueAsset->GetReturnDialogue();
	}

	/* stop any timers that are running */
	if (GetWorld())
	{
		float ElapsedTime = GetWorld()->GetTimerManager().GetTimerRemaining(DialogueTimerHandle);
		if (ElapsedTime > 0.0F)
		{
			GetWorld()->GetTimerManager().ClearTimer(DialogueTimerHandle);
		}
	}

	bConversationRunning = false;
	OnExitConversation.ExecuteIfBound(InstigatingCharacter);
	InstigatingCharacter->GetDialogueWidget()->EndDialogue();
	InstigatingCharacter = false;
}

void UDialogueComponent::SendInput(FName Input /*= NAME_None*/)
{
	// timer
	if (GetWorld() && CurrentDialogueAsset)
	{
		float ElapsedTime = GetWorld()->GetTimerManager().GetTimerRemaining(DialogueTimerHandle);
		if (ElapsedTime > 0.0F || FMath::IsNearlyEqual(ElapsedTime, -1.0F))
		{
			if (CurrentDialogueAsset->SendInput(Input))
			{
				GetWorld()->GetTimerManager().ClearTimer(DialogueTimerHandle);
				RunDialogueAsset(CurrentDialogueAsset->GetNextDialogue());
			}
		}
	}
}

// Called when the game starts
void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDialogueComponent::DialogueTimerCallback()
{
	if(CurrentDialogueAsset)
		RunDialogueAsset(CurrentDialogueAsset->GetNextDialogue());
}

