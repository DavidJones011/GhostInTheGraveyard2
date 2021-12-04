// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeadBobComponent.generated.h"

class UCurveVector;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GHOSTINTHEGRAVEYARD_API UHeadBobComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHeadBobComponent();

private:

	UPROPERTY()
	float BaseTimeAccumulator;

	UPROPERTY()
	float AddTimeAccumulator;

	UPROPERTY()
	FVector CameraRelativeLocationStart;

	UPROPERTY(Transient)
	bool bPlayingAdditiveCurve = false;

	UPROPERTY(EditAnywhere)
	TMap<FName, UCurveVector*> AdditiveCurves;

	FTimerHandle AdditiveCurveTimerHandle;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UCurveVector* IdleCurve;

	UPROPERTY(EditAnywhere)
	UCurveVector* WalkCurve;

	UPROPERTY(EditAnywhere)
	UCurveVector* SprintCurve;

	UPROPERTY(Transient)
	UCurveVector* CurrentAdditiveCurve;

	UPROPERTY(EditAnywhere)
	float SprintToleranceSquared = 170000.0F;

	virtual FVector StepVectorCurve(const UCurveVector* InCurve, float DeltaTime, bool bLoop = true);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetCameraRelativeLocationStart(const FVector& Location) { CameraRelativeLocationStart = Location; }

	void PlayAdditiveCurve(const FName& Name);

	void EndAdditiveAnim() { bPlayingAdditiveCurve = false; }
};
