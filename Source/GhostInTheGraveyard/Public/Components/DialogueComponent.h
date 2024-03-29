// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueComponent.generated.h"

class UDialogueAsset;
class ASurvivorCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GHOSTINTHEGRAVEYARD_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	/*
	* The dialogue asset to be used when the player initiates dialogue.
	*/
	UPROPERTY(EditAnywhere)
	UDialogueAsset* StartingDialogueAsset = nullptr;

	UPROPERTY(Transient)
	UDialogueAsset* CurrentDialogueAsset = nullptr;

	UPROPERTY(Transient)
	bool bConversationRunning = false;

	FTimerHandle DialogueTimerHandle;

	UPROPERTY(Transient)
	ASurvivorCharacter* InstigatingCharacter = nullptr;

public:	
	// Sets default values for this component's properties
	UDialogueComponent();

	UFUNCTION(BlueprintCallable)
	void StartConversation(ASurvivorCharacter* Instigator);

	UFUNCTION()
	void ExitConversation();

	UFUNCTION(BlueprintCallable)
	void SendInput(FName Input = NAME_None);

	UFUNCTION(BlueprintCallable)
	bool ConversationIsRunning() const { return bConversationRunning; }

	DECLARE_DELEGATE_OneParam(FExitConversation, ASurvivorCharacter*);
	FExitConversation OnExitConversation;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;
 
 	UFUNCTION()
 	void DialogueTimerCallback();

	UFUNCTION()
	void RunDialogueAsset(UDialogueAsset* Dialogue);
};
