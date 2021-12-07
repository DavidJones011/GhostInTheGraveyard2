// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GhostInTheGraveyardCharacter.h"
#include "CreatureCharacter.generated.h"

UCLASS(config = Game)
class GHOSTINTHEGRAVEYARD_API ACreatureCharacter : public AGhostInTheGraveyardCharacter
{
	GENERATED_BODY()

private:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* AudioComponent = nullptr;

	UPROPERTY(EditAnywhere)
	float SoundAttenuationMaxRange = 2500.0F;

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

	bool CheckForActorInFront(float Distance, FHitResult& OutResult);

	UFUNCTION(BlueprintCallable)
	UAudioComponent* GetAudioComponent() const { return AudioComponent; }

};
