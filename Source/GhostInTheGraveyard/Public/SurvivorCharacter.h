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

	
private:

	/** First person camera */
	

	
	const FVector cameraNormalPosition = FVector(0.0f,0.0f,70.0f);
	const FVector cameraHidePosition = FVector(0.0f, 0.0f, 0.0f);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;


public:
	IInteractable* currentInteract;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;
	

	UPROPERTY(BlueprintReadWrite)
		bool Hidden;

	UPROPERTY(BlueprintReadWrite)
		bool Trapped;

	UPROPERTY(BlueprintReadWrite)
		bool CanInteract;


protected:

	virtual void Tick(float deltaSeconds) override;

	virtual void BeginPlay();

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

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface




public: 


	// Hiding
	bool Hide(AHidingSpot* spot);
	void Leave(AHidingSpot* spot);
	void EscapeTrap(ATrap* trap);
	virtual void Jump() override;

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/* This helps the AI detect the character in a custom way.*/
	virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor = NULL) const override;


	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void Interact();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void EndInteract();

	UFUNCTION(BlueprintCallable, Category = "Interacts")
		bool Trap(ATrap* trap);

	
};
