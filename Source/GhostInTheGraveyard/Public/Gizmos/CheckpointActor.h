// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckpointActor.generated.h"

UENUM(BlueprintType)
enum ECheckpointState
{
	CPS_NOT_VISITED = 0 UMETA(DisplayName = "Not Visited"),
	CPS_VISITED     = 1 UMETA(DisplayName = "Visited"),
	CPS_ACTIVE      = 2 UMETA(DisplayName = "Active")
};

UCLASS()
class GHOSTINTHEGRAVEYARD_API ACheckpointActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpointActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<ECheckpointState> CheckpointState = ECheckpointState::CPS_NOT_VISITED;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
 
 	UFUNCTION(BlueprintNativeEvent)
 	void ChangeCheckPointState(ECheckpointState State);
 	virtual void ChangeCheckPointState_Implementation(ECheckpointState State){};
 
 	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECheckpointState GetCheckpointState() const { return CheckpointState; }
};
