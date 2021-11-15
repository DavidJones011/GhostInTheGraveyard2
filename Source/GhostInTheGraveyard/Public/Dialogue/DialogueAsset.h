// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Internationalization/Text.h"
#include "DialogueAsset.generated.h"

class USoundBase;

// /**
//  * Enum that the dialogue component can use to determine how to wait for input.
//  */
// UENUM(Blueprintable)
// enum ERequiredInputType
// {
// 	RIT_UserInterface,
// 	RIT_Inventory,
// 	RIT_CharacterInput,
// 	RIT_None
// };

/**
 * 
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API UDialogueAsset : public UDataAsset
{
	GENERATED_BODY()

protected:

	/* The dialogue that should be returned to when exited early. */
	UPROPERTY(EditAnywhere)
	UDialogueAsset* ReturnPointDialogue = nullptr;

	UPROPERTY(EditAnywhere)
	FName WidgetName = "Subtitle";

public:

	/*
	* Gets the sound asset of the dialogue.
	*/
	UFUNCTION(BlueprintCallable)
	virtual USoundBase* GetSound() const { return nullptr; }

	/**
	 * Gets the duration of the dialogue.
	 */
	UFUNCTION(BlueprintCallable)
	virtual float GetTimeDuration() const { return 0.0F; }

	/*
	* Gets the text for the dialogue.
	*/
	UFUNCTION(BlueprintCallable)
	virtual FText GetDialogueText() const { return FText::FromString("Default Dialogue Text"); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetWidgetName() const { return WidgetName; }

	/**
	 * Gets the returning dialogue asset.
	 * This is used when a conversation is canceled or ends.
	 */
	UFUNCTION(BlueprintCallable)
	virtual UDialogueAsset* GetReturnDialogue() const { return ReturnPointDialogue; }

	/**
	 * Get the next dialogue and the index of that asset.
	 */
	UFUNCTION()
	virtual UDialogueAsset* GetNextDialogue() { return nullptr; }

	/**
	 * Send an input to the current dialogue. Returns if the input was successful or not.
	 */
	UFUNCTION()
	virtual bool SendInput(FName Input = NAME_None) { return false; }

	/*
	* Called when the dialogue asset has been entered for the first time.
	* Can be used to initialize data.
	*/
	UFUNCTION()
	virtual void OnDialogueEnter() {};

	/*
	* Called when the dialogue asset has been exited.
	* Can be used to reset data.
	*/
	UFUNCTION()
	virtual void OnDialogueExit() {};
};
