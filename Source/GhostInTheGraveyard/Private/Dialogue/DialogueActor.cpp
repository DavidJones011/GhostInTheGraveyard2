// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue/DialogueActor.h"
#include "Components/DialogueComponent.h"
#include "Gameframework/PawnMovementComponent.h"

// Sets default values
ADialogueActor::ADialogueActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DialogueComponent = CreateDefaultSubobject<UDialogueComponent>(TEXT("Dialogue Component"));
	DialogueComponent->OnExitConversation.BindUFunction(this, "ExitedConversation");
}

// Called when the game starts or when spawned
void ADialogueActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADialogueActor::ExitedConversation(ASurvivorCharacter* CharacterInstigator)
{
	CharacterInstigator->SetInteractingDialogueActor(nullptr);
}

// Called every frame
void ADialogueActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADialogueActor::Interact(ASurvivorCharacter* player)
{
	if(!DialogueComponent)
		return;

	if (DialogueComponent->ConversationIsRunning())
	{
		DialogueComponent->SendInput("Skip");
	}
	else
	{
		player->SetInteractingDialogueActor(this);
		player->GetMovementComponent()->StopMovementImmediately();
		player->GetMovementComponent()->ConsumeInputVector();
		DialogueComponent->StartConversation(player);
	}
}

void ADialogueActor::EndInteract(ASurvivorCharacter* player)
{
	
}

