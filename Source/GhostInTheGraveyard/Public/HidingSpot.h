// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "SurvivorCharacter.h"

#include "HidingSpot.generated.h"

UCLASS()
class GHOSTINTHEGRAVEYARD_API AHidingSpot : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AHidingSpot();

private:
	UStaticMesh hide;
	USphereComponent hidingPoint;
	USphereComponent outPoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Hide(ASurvivorCharacter * player);
	void UnHide(ASurvivorCharacter * player);
};
