// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"
#include "Components/DetectorComponent.h"

struct FCreatureData
{
	TArray<FDetectionResult> DetectedData;
};

class GHOSTINTHEGRAVEYARDEDITOR_API FGameplayDebuggerCategory_Creature : public FGameplayDebuggerCategory
{
protected:

	FCreatureData Data;

public:

	FGameplayDebuggerCategory_Creature();

	/** Creates an instance of this category - will be used on module startup to include our category in the Editor */
	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

	/** Collects the data we would like to print */
	virtual void CollectData(class APlayerController* OwnerPC, class AActor* DebugActor) override;

	/** Displays the data we collected in the CollectData function */
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;
};