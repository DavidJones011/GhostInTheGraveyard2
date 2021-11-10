// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueComponent.generated.h"

class UDialogueAsset;

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

	FTimerHandle DialogueTimerHandle;

public:	
	// Sets default values for this component's properties
	UDialogueComponent();

	UFUNCTION(BlueprintCallable)
	void StartConversation();

	UFUNCTION()
	void ExitConversation();

	UFUNCTION(BlueprintCallable)
	void SendInput(FName Input = NAME_None);

protected:

	// Called when the game starts
	virtual void BeginPlay() override;
 
 	UFUNCTION()
 	void DialogueTimerCallback();

	UFUNCTION()
	void RunDialogueAsset(UDialogueAsset* Dialogue);
};
