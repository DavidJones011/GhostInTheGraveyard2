// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:

	UFUNCTION(BlueprintNativeEvent)
	void SetInteractMessage(const FText& Text);
	virtual void SetInteractMessage_Implementation(const FText& Text){};

	UFUNCTION(BlueprintNativeEvent)
	void OnFocusBegin();
	virtual void OnFocusBegin_Implementation(){};

	UFUNCTION(BlueprintNativeEvent)
	void OnFocusEnd();
	virtual void OnFocusEnd_Implementation(){};
};
