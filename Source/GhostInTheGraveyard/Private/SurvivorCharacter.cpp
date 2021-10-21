// Copyright Epic Games, Inc. All Rights Reserved.


#include "SurvivorCharacter.h"
#include "GhostInTheGraveyardProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h"
#include "CollisionQueryParams.h"
#include "AI/AIDirectorSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AGhostInTheGraveyardCharacter

ASurvivorCharacter::ASurvivorCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;


	// Create a CameraComponent
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera

}

//TODO: Implemented this way for now, but should change in the future to fit the games design
bool ASurvivorCharacter::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor /*= NULL*/)  const
{
	// perform ray traces to determine if the character is behind a wall
	ECollisionChannel DefaultSightCollisionChannel = ECollisionChannel::ECC_WorldDynamic;
	FHitResult HitResult;
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, ObserverLocation, GetActorLocation()
		,DefaultSightCollisionChannel
		,FCollisionQueryParams("", true, IgnoreActor));

	bool bSuccess = false;
	OutSightStrength = 0.0;
	NumberOfLoSChecksPerformed = 1;
	if (bHit && HitResult.Actor == this)
	{
		OutSightStrength = 1.0F;
		bSuccess = true;
	}

	return bSuccess;
}

void ASurvivorCharacter::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();


	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}

	UWorld* World = GetWorld();
	if (World)
	{
		UAIDirectorSubsystem* Director = GetWorld()->GetSubsystem<UAIDirectorSubsystem>();
		Director->RegisterPlayer(this);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASurvivorCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASurvivorCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ASurvivorCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASurvivorCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASurvivorCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASurvivorCharacter::LookUpAtRate);
}

void ASurvivorCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}




void ASurvivorCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASurvivorCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASurvivorCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASurvivorCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
