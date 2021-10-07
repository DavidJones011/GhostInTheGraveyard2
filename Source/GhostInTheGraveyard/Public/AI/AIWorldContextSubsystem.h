// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AIWorldContextSubsystem.generated.h"


class ACreatureAIController;
class AAIPointContextManager;

/**
 * 
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API UAIWorldContextSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	UAIWorldContextSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection);

	void RegisterAIContextManager(AAIPointContextManager* Manager);

	void RegisterAIController(ACreatureAIController* Controller);

	/*
	* Send the tracked AI controller to the section.
	*/
	UFUNCTION(BlueprintCallable)
	void SendAIToPatrolSection(int32 Section, bool bTeleport = false);

	/*
	* Send the tracked AI controller to a specific patrol point.
	*/
	UFUNCTION(BlueprintCallable)
	void SendAIToPatrolPoint(int32 Section, int32 Index, bool bTeleport = false);

	// inlines
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsTrackingAIController() const { return TrackedController != nullptr; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ACreatureAIController* GetAIController() { return TrackedController; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AAIPointContextManager* GetAIContextManager() { return TrackedManager; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool HasValidAIContextManager() const { return TrackedManager != nullptr; }

	ACreatureAIController* TrackedController;
	AAIPointContextManager* TrackedManager;
};
