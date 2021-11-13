// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API UDialogueUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void SetDialogueText(const FText& InText);

	virtual void SetDialogueText_Implementation(FText InText) {};

	UFUNCTION(BlueprintNativeEvent)
	void BeginDialogue();

	virtual void BeginDialogue_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
	void EndDialogue();

	virtual void EndDialogue_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
	void SetDialogueWidget(FName WidgetName);

	virtual void SetDialogueWidget_Implementation(FName WidgetName) {};
};
