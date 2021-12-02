// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Components/PatrolTrackerComponent.h"
#include "AIDirectorSubsystem.generated.h"

class ASurvivorCharacter;
class ACreatureAIController;
class AAIPointContextManager;

USTRUCT(Blueprintable)
struct FRecordAIState
{
	GENERATED_BODY()

	UPROPERTY()
	FVector PlayerLocation;
	
	UPROPERTY()
	FRotator PlayerRotation;

	UPROPERTY()
	int32 AISection;

	UPROPERTY()
	FVector AICharacterLocation;

	TWeakObjectPtr<AActor> LastCheckpoint;
};

/**
 * 
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API UAIDirectorSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()	

public:

	UAIDirectorSubsystem();

	UFUNCTION(BlueprintCallable)
	void RecordState(AActor* CheckpointActor);

	UFUNCTION(BlueprintCallable)
	void LoadRecordedState();

	/*
	* Register the player for the AI director to track.
	*/
	UFUNCTION(BlueprintCallable)
	void RegisterPlayer(ASurvivorCharacter* Character);

	/*
	* Unregister the currently tracked player.
	*/
	UFUNCTION(BlueprintCallable)
	void UnregisterPlayer();

	/*
	* Register the AI Controller to be receiving data.
	*/
	UFUNCTION(BlueprintCallable)
	void RegisterAIController(ACreatureAIController* Controller);

	/*
	* Unregister the current AI controller receiving data.
	*/
	UFUNCTION(BlueprintCallable)
	void UnregisterAIController();

	/*
	* Register the AI context manager for the AI to use.
	*/
	UFUNCTION(BlueprintCallable)
	void RegisterAIContextManager(AAIPointContextManager* Manager);

	/*
	* Unregister the current AI context manager.
	*/
	UFUNCTION(BlueprintCallable)
	void UnregisterAIContextManager();

	/*
	* Updates the patrol section the tracked AI should be in.
	*/
	UFUNCTION(BlueprintCallable)
	void SendAIToPatrolSection(int32 Section, float Delay = 0.0F, EPatrolTraversalMode TraverseMode = EPatrolTraversalMode::Loop, bool bTeleport = false) { SendAIToPatrolPoint(Section, -1, Delay, TraverseMode, bTeleport); }

	/*
	* Has the AI detect the given actor instantly.
	*/
	UFUNCTION(BlueprintCallable)
	void SetAIAwareOfActor(AActor* DetectedActor);

	/* Sets the AI to be aware of the player instantly */
	UFUNCTION(BlueprintCallable)
	void SetAIAwareOfPlayer();

	/* Have the AI forget the player. */
	UFUNCTION(BlueprintCallable)
	void SetAIForgetPlayer();

	/*
	* Has the AI investigate the location right away.
	*/
	UFUNCTION(BlueprintCallable)
	void SendAIToInvestigateLocation(FVector Location);

	UFUNCTION(BlueprintCallable)
	void SendAIToInvestigatePlayerLocation();

	/*
	* Updates the patrol section the tracked AI should be in.
	*/
	UFUNCTION(BlueprintCallable)
	void SendAIToPatrolPoint(int32 Section, int32 Index, float Delay = 0.0F, EPatrolTraversalMode TraverseMode = EPatrolTraversalMode::Loop, bool bTeleport = false);

	/*
	* Teleports the AI to the specific patrol section and index.
	* If the index = -1, then it teleports the AI to the nearest index of that section.
	*/
	UFUNCTION(BlueprintCallable)
	void PlaceAIAtPatrolPoint(int32 Section, int32 Index);

protected:

	UFUNCTION(BlueprintCallable)
	void SendAIToPatrolPoint_Impl(int32 Section, int32 Index, EPatrolTraversalMode TraverseMode, bool bTeleport);

	FTimerHandle TimerHandle;

private:

	UPROPERTY(Transient)
	ASurvivorCharacter* PlayerCharacter;

	UPROPERTY(Transient)
	ACreatureAIController* AIController;

	UPROPERTY(Transient)
	AAIPointContextManager* PatrolManager;

	UPROPERTY()
	FRecordAIState LastRecord;
};