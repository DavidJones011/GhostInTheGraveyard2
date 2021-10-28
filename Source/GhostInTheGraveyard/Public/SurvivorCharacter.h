// Fill out your copyright notice in the Description page of Project Settings.

#pragma once




#include "GhostInTheGraveyardCharacter.h"
#include "Perception/AISightTargetInterface.h"

#include "SurvivorCharacter.generated.h"

class IInteractable;
class AHidingSpot;
class ATrap;

UCLASS(config = Game)
class GHOSTINTHEGRAVEYARD_API ASurvivorCharacter : public AGhostInTheGraveyardCharacter, public IAISightTargetInterface
{
	GENERATED_BODY()

public:

	ASurvivorCharacter(const FObjectInitializer& ObjectInitializer);

	/* This helps the AI detect the character in a custom way.*/
	virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor = NULL) const override;

private:

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	IInteractable* currentInteractable;
	const FVector cameraNormalPosition = FVector(0.0f,0.0f,70.0f);
	const FVector cameraHidePosition = FVector(0.0f, 0.0f, 0.0f);


protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;
protected:

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface


public:
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }



	// Game Logic Functions and variables

	//Interaction
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact();
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void EndInteract();



	UPROPERTY(EditAnywhere)
	int InteractRange = 500;

	// Hiding
	bool Hide(AHidingSpot* spot);
	void Leave(AHidingSpot* spot);

	UPROPERTY(BlueprintReadOnly)
	bool Hidden;


	UFUNCTION(BlueprintCallable,Category="GetFukd")
	bool Trap(ATrap* trap);

	void EscapeTrap(ATrap* trap);
	bool Trapped;

	virtual void Jump() override;
};
