// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue/DialogueActor.h"
#include "Components/DialogueComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/AIDirectorSubsystem.h"

// Sets default values
ADialogueActor::ADialogueActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
	if (GetWorld())
	{
		UAIDirectorSubsystem* Director = GetWorld()->GetSubsystem<UAIDirectorSubsystem>();
		if (Director) Director->SetLoadCheckpointEnabled(true);
	}

	CharacterInstigator->SetInteractingDialogueActor(nullptr);
	CharacterInstigator->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	CharacterInstigator->currentInteract = nullptr;
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
		if (GetWorld())
		{
			UAIDirectorSubsystem* Director = GetWorld()->GetSubsystem<UAIDirectorSubsystem>();
			if (Director) Director->SetLoadCheckpointEnabled(false);
		}

		player->SetInteractingDialogueActor(this);
		player->GetCharacterMovement()->StopMovementImmediately();
		player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		DialogueComponent->StartConversation(player);
	}
}

void ADialogueActor::EndInteract(ASurvivorCharacter* player)
{
	
}

