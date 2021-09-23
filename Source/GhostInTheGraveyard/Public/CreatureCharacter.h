// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GhostInTheGraveyardCharacter.h"
#include "CreatureCharacter.generated.h"

UCLASS(config = Game)
class GHOSTINTHEGRAVEYARD_API ACreatureCharacter : public AGhostInTheGraveyardCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACreatureCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
