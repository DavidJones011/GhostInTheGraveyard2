// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Components/PatrolTrackerComponent.h"
#include "AIDirectorSubsystem.generated.h"

class ASurvivorCharacter;
class ACreatureAIController;
class AAIPointContextManager;

/**
 * 
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API UAIDirectorSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()	

public:

	UAIDirectorSubsystem();

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
	* Updates the patrol section the tracked AI should be in.
	*/
	UFUNCTION(BlueprintCallable)
	void SendAIToPatrolPoint(int32 Section, int32 Index, float Delay = 0.0F, EPatrolTraversalMode TraverseMode = EPatrolTraversalMode::Loop, bool bTeleport = false);

protected:

	UFUNCTION(BlueprintCallable)
	void SendAIToPatrolPoint_Impl(int32 Section, int32 Index, EPatrolTraversalMode TraverseMode, bool bTeleport);

	FTimerHandle TimerHandle;

private:

	UPROPERTY()
	ASurvivorCharacter* PlayerCharacter;

	UPROPERTY()
	ACreatureAIController* AIController;

	UPROPERTY()
	AAIPointContextManager* PatrolManager;
};