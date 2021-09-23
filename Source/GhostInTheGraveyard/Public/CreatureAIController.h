// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CreatureAIController.generated.h"

UENUM(BlueprintType)
enum EInvestigateState
{
	Check            = 0 UMETA(DisplayName = "Check"),
	Scattered_Search = 1 UMETA(DisplayName = "Scattered_Search"),
	Thorough_Search  = 2 UMETA(DisplayName = "Thorough_Search"),
	None             = 3 UMETA(DisplayName = "None")
};

/**
 * 
 */
UCLASS(config = Game)
class GHOSTINTHEGRAVEYARD_API ACreatureAIController : public AAIController
{
	GENERATED_BODY()

public:

	ACreatureAIController(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	// Used for the location and rotation for the Perception Component
	virtual void GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const override;

	// Reports the EQS Query Result
	virtual void ReportEQSQueryResult(TSharedPtr<struct FEnvQueryResult> Result);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UDetectorComponent* GetDetectorComponent() { return DetectorComponent;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE	UBehaviorTreeComponent* GetBehaviorTreeComponent() { return BehaviorTreeComponent; }

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnDetectedUpdate(AActor* DetectedActor, uint32 Stage);

private:

	UFUNCTION()
	void OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
	void OnTargetPerceptionUpdate(AActor* InActor, const FAIStimulus Stimulus);

private:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UDetectorComponent* DetectorComponent;

	UPROPERTY()
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere)
	class UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY(EditAnywhere)
	class UAISenseConfig_Sight* SightConfig;
};