// Fill out your copyright notice in the Description page of Project Settings.


#include "GameDebuggers/GameplayDebuggerCategory_Creature.h"
#include "CreatureAIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

FGameplayDebuggerCategory_Creature::FGameplayDebuggerCategory_Creature()
{
	bShowOnlyWithDebugActor = false;
}

TSharedRef<FGameplayDebuggerCategory> FGameplayDebuggerCategory_Creature::MakeInstance()
{
	return MakeShareable(new FGameplayDebuggerCategory_Creature());
}

void FGameplayDebuggerCategory_Creature::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	if (DebugActor == NULL)
		return;

	ACharacter* Character = Cast<ACharacter>(DebugActor);
	if(Character == NULL)
		return;

	ACreatureAIController* CreatureController = Cast<ACreatureAIController>(Character->GetController());
	if (CreatureController)
	{
		UDetectorComponent* DetectorComponent = CreatureController->GetDetectorComponent();
		if (DetectorComponent)
		{
			TArray<FDetectionResult> DetectionResults;
			DetectorComponent->GetDetectedArray(DetectionResults, EDetectionStage::Unaware);
			Data.DetectedData = DetectionResults;
		}
	}
}

void FGameplayDebuggerCategory_Creature::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	CanvasContext.Print(TEXT("Detection:"));

	for (FDetectionResult& DetectionData : Data.DetectedData)
	{
		FColor DisplayColor = FColor::Cyan;
		switch (DetectionData.Stage)
		{
		case EDetectionStage::Aware:
			DisplayColor = FColor::Red;
			break;
		case EDetectionStage::Cautious:
			DisplayColor = FColor::Yellow;
			break;
		case EDetectionStage::Curious:
			DisplayColor = FColor::Orange;
			break;
		default:
			break;
		}

		CanvasContext.Print(DisplayColor, FString::Printf(TEXT("%s: %.1f"), *DetectionData.Actor->GetName(), DetectionData.Value));
	}
}
