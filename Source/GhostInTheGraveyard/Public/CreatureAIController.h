// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CreatureAIController.generated.h"

class USoundBase;
class AAmbientSound;

UENUM(BlueprintType)
enum ECreatureState
{
	ST_Patrol            = 0 UMETA(DisplayName = "Patrol"),
	ST_Search            = 1 UMETA(DisplayName = "Search"),
	ST_Investigate       = 2 UMETA(DisplayName = "Investigate"),
	ST_Pursue            = 3 UMETA(DisplayName = "Pursue"),
	ST_InvalidState      = 4 UMETA(DisplayName = "Invalid_State")
};

/**
 * 
 */
UCLASS(config = Game)
class GHOSTINTHEGRAVEYARD_API ACreatureAIController : public AAIController
{
	GENERATED_BODY()

private:

	UPROPERTY(Transient)
	float LastHeardSound = -100.0F;

	UPROPERTY(EditAnywhere)
	float AIBarkTimeNeeded = 5.0F;

	UPROPERTY(Transient)
	float CurrentHearingScore = 0.0F;

	UPROPERTY(EditAnywhere)
	float HearingForgetRate = 10.0F;

	UPROPERTY(EditAnywhere)
	float TimeNeededToPlayIdleBark = 90.0F;

	UPROPERTY(Transient)
	float LastTimeIdleBarkPlayed = 0.0F;

	UPROPERTY(EditAnywhere)
	AAmbientSound* ChaseMusicSoundActor = nullptr;

public:

	ACreatureAIController(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	// Used for the location and rotation for the Perception Component
	virtual void GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const override;

	virtual void ResetHearingScore() { CurrentHearingScore = 0.0F;}

	// Reports the EQS Query Result
	virtual void ReportEQSQueryResult(TSharedPtr<struct FEnvQueryResult> Result);

	UFUNCTION(BlueprintCallable)
	bool CanBeTeleported() const;

	/**
	 * Send the tracked AI controller to go to the given patrol point.
	 */
	UFUNCTION(BlueprintCallable)
	bool SendAIToPatrolPoint(AAIPointContextManager* Manager, int32 Section, int32 Index, bool bTeleport = false);

	/**
	 * Send the tracked AI controller to go to the given patrol section.
	 */
	UFUNCTION(BlueprintCallable)
	bool SendAIToPatrolSection(AAIPointContextManager* Manager, int32 Section, bool bTeleport = false);

	UFUNCTION(BlueprintCallable)
	FVector UpdateNextPatrolPoint();

	UFUNCTION(BlueprintCallable)
	void InstantlyDetect(AActor* InActor);

	UFUNCTION(BlueprintCallable)
	void InvestigateLocation(FVector Location);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UDetectorComponent* GetDetectorComponent() { return DetectorComponent;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UPatrolTrackerComponent* GetPatrolTrackerComponent() { return PatrolTrackerComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE	UBehaviorTreeComponent* GetBehaviorTreeComponent() { return BehaviorTreeComponent; }

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnDetectedUpdate(AActor* DetectedActor, uint32 Stage);

	UPROPERTY(EditAnywhere)
	USoundBase* FoundAIBark;

	UPROPERTY(EditAnywhere)
	USoundBase* CuriousAIBark;

	UPROPERTY(EditAnywhere)
	USoundBase* HeardAIBark;

	UPROPERTY(EditAnywhere)
	USoundBase* IdleBark;

private:

	UFUNCTION()
	void OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
	void OnTargetPerceptionUpdate(AActor* InActor, const FAIStimulus Stimulus);

	void PlayHearBark();

private:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UDetectorComponent* DetectorComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UPatrolTrackerComponent* PatrolTrackerComponent;

	UPROPERTY()
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Sight* SightConfig;
};