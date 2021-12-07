// Copyright Epic Games, Inc. All Rights Reserved.


#include "SurvivorCharacter.h"
#include "Interactable.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h"
#include <Runtime/Engine/Classes/GameFramework/SpringArmComponent.h>
#include "XRMotionControllerBase.h"
#include "CollisionQueryParams.h"
#include "AI/AIDirectorSubsystem.h"
#include "HidingSpot.h"
#include "Perception/AISense_Hearing.h"
#include "Dialogue/DialogueUserWidget.h"
#include "Components/InventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gizmos/Trap.h"
#include "InteractionWidget.h"
#include "GameMenu_UserWidget.h"
#include "Components/HeadBobComponent.h"
#include "AI/AIStrings.h"
#include "Components/DialogueComponent.h"
#include "Dialogue/DialogueActor.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Sound/AmbientSound.h"

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

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
	GetCharacterMovement()->bApplyGravityWhileJumping = false;

	HeadBobComponent = CreateDefaultSubobject<UHeadBobComponent>(TEXT("HeadBob Component"));
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
		if (Hidden) {
			OutSightStrength = 0.0f;
			bSuccess = true;
		}
		else {
		
			float Ratio = FMath::Clamp((ObserverLocation - GetActorLocation()).Size() / 4000.0F, 0.0F, 1.0F);
			OutSightStrength = FMath::Lerp(1.0F, 0.0F, Ratio);
			bSuccess = true;
		}
	}

	return bSuccess;
}

void ASurvivorCharacter::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();

	StepRate = WalkStepRate;
	StepTimer = StepRate;

	UWorld* World = GetWorld();
	if (World)
	{
		// register to AI director
		UAIDirectorSubsystem* Director = GetWorld()->GetSubsystem<UAIDirectorSubsystem>();
		Director->RegisterPlayer(this);

		// create the dialogue widget
		if (IsPlayerControlled())
		{
			//Cast<APlayerController>(GetController())->SetTickableWhenPaused(true);
			//SetTickableWhenPaused(true);

			if (DialogueWidgetClass)
			{
				DialogueWidget = Cast<UDialogueUserWidget>(CreateWidget(GetWorld(), DialogueWidgetClass));
				if(DialogueWidget)
				{
					DialogueWidget->AddToViewport();
					DialogueWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
			
			if (InteractWidgetClass)
			{
				InteractWidget = Cast<UInteractionWidget>(CreateWidget(GetWorld(), InteractWidgetClass));
				if (InteractWidget)
				{
					InteractWidget->AddToViewport();
					InteractWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}

			if (PauseMenuWidgetClass)
			{
				PauseGameWidget = Cast<UGameMenu_UserWidget>(CreateWidget(GetWorld(), PauseMenuWidgetClass));
				if (PauseGameWidget)
				{
					PauseGameWidget->AddToViewport();
					PauseGameWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}

			if (DeathMenuWidgetClass)
			{
				DeathWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), DeathMenuWidgetClass));
				if (DeathWidget)
				{
					DeathWidget->AddToViewport();
					DeathWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASurvivorCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Pause Game
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &ASurvivorCharacter::PauseGame);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASurvivorCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Bind sprint events
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASurvivorCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASurvivorCharacter::StopSprint);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASurvivorCharacter::OnResetVR);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASurvivorCharacter::Interact);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &ASurvivorCharacter::EndInteract);


	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ASurvivorCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASurvivorCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &ASurvivorCharacter::Turn);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASurvivorCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ASurvivorCharacter::Lookup);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASurvivorCharacter::LookUpAtRate);
}

void ASurvivorCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASurvivorCharacter::Tick(float DeltaSeconds) {

	// update foot step sounds

	if (GetCharacterMovement())
	{
		//update sprinting
		if (bSprinting)
		{
			FVector LastInput = GetCharacterMovement()->GetLastInputVector();
			float SprintDot = GetActorForwardVector() | LastInput;
			if (SprintDot > 0.7F && !bLastFrameSprinting)
			{
				GetCharacterMovement()->MaxWalkSpeed = 600.0F;
			}
			else if(SprintDot <= 0.7F && bLastFrameSprinting)
			{
				GetCharacterMovement()->MaxWalkSpeed = 400.0F;
			}
		}
		else if(!bSprinting && bLastFrameSprinting)
		{
			GetCharacterMovement()->MaxWalkSpeed = 400.0F;
		}
		bLastFrameSprinting = bSprinting;

		// foot step sounds
		if (GetCharacterMovement()->IsMovingOnGround() && GetCharacterMovement()->Velocity.SizeSquared() > 0.0F)
		{
			StepTimer -= DeltaSeconds;
			if (StepTimer <= 0.0F)
			{
				StepTimer += StepRate;
				float SoundRange = bSprinting ? 700.0F : 400.0F;
				float Loudness = bSprinting ? 600.0F : 300.0F;
				float Volume = bSprinting ? 0.45F : 0.25F;
				UAISense_Hearing::ReportNoiseEvent(GetWorld(), this->GetActorLocation(), Loudness, this, SoundRange, FAIPerceptionTags::NoiseTag);
				if (FootStepSounds.Num() > 0)
				{
					int32 Index = FMath::FRandRange(0, FootStepSounds.Num() - 1);

					// reduce the chance of hearing the same footstep multiple times in a row
					if (PrevFootStepIndexTaken == Index)
					{
						Index = FMath::FRandRange(0, FootStepSounds.Num() - 1);
					}

					USoundBase* FoostepSound = FootStepSounds[Index];
					UGameplayStatics::PlaySound2D(GetWorld(), FoostepSound, Volume);
					PrevFootStepIndexTaken = Index;
				}
			}
		}
		else
		{
			StepTimer = StepRate;
		}
	}


	// update interaction
	if (Hidden || Trapped)
		return;

	FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector End = Start + GetFirstPersonCameraComponent()->GetForwardVector() * 300.0F;
	FHitResult Hit;
	FCollisionQueryParams QueryParams = FCollisionQueryParams("Interact", false, this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_WorldDynamic, QueryParams);

	if (bHit)
	{
		IInteractable* Interactable = Cast<IInteractable>(Hit.Actor);
		if (Interactable && Interactable->CanInteract(this))
		{
			if (InteractWidget && currentInteract != Interactable)
			{
				InteractWidget->SetInteractMessage(FText::FromString(Interactable->GetInteractionMessage(this)));
				InteractWidget->SetVisibility(ESlateVisibility::Visible);
			}

			CanInteract = true;
			currentInteract = Interactable;
		}
		else
		{
			if (InteractWidget && currentInteract)
			{
				InteractWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			CanInteract = false;
			currentInteract = nullptr;
		}
	}
	else if (Trapped && InteractWidget) 
	{
		InteractWidget->SetInteractMessage(FText::FromString(currentInteract->GetInteractionMessage(this)));
		InteractWidget->SetVisibility(ESlateVisibility::Visible);
	} 
	else
	{
		if (InteractWidget && currentInteract)
		{
			InteractWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		CanInteract = false;
		currentInteract = nullptr;
	}
}

void ASurvivorCharacter::MoveForward(float Value)
{
	if (GetInteractingDialogueActor())
		return;

	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
		//UAISense_Hearing::ReportNoiseEvent(GetWorld(), this->GetActorLocation(), 0.5, this, 600.0f, FAIPerceptionTags::NoiseTag);
	}
}

void ASurvivorCharacter::MoveRight(float Value)
{
	if (GetInteractingDialogueActor())
		return;

	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
		//UAISense_Hearing::ReportNoiseEvent(GetWorld(), this->GetActorLocation(), 0.5, this, 600.0f, FAIPerceptionTags::NoiseTag);
	}
}

void ASurvivorCharacter::PauseGame()
{
	APlayerController* PlayerCont = Cast<APlayerController>(GetController());

	if(!PlayerCont)
		return;
	
	if (!PlayerCont->IsLocalPlayerController())
		return;

	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);	
		if (PauseGameWidget)
		{
			PauseGameWidget->SetVisibility(ESlateVisibility::Hidden);
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerCont);
			PlayerCont->bShowMouseCursor = false;	
		}
	}
	else
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		if (PauseGameWidget)
		{
			PauseGameWidget->SetVisibility(ESlateVisibility::Visible);
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerCont, PauseGameWidget, EMouseLockMode::DoNotLock);
			PauseGameWidget->OnPaused();
			PlayerCont->bShowMouseCursor = true;
		}
	}
}

void ASurvivorCharacter::Turn(float Val)
{
	if (GetInteractingDialogueActor())
		return;

	AddControllerYawInput(Val);
}

void ASurvivorCharacter::Lookup(float Val)
{
	if (GetInteractingDialogueActor())
		return;

	AddControllerPitchInput(Val);
}

void ASurvivorCharacter::TurnAtRate(float Rate)
{
	if (GetInteractingDialogueActor())
		return;
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASurvivorCharacter::LookUpAtRate(float Rate)
{
	if (GetInteractingDialogueActor())
		return;
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASurvivorCharacter::Interact()
{
	if (currentInteract) {
		//if (GEngine)
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Interacting"));
		if (InteractWidget && !(Trapped || Hidden))
		{
			InteractWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		currentInteract->Interact(this);
	}
}

void ASurvivorCharacter::EndInteract()
{
	if (currentInteract) {
		currentInteract->EndInteract(this);
	}
}

bool ASurvivorCharacter::Hide(AHidingSpot* spot)
{
	if (!Hidden) {
		Hidden = true;
		GetController()->SetIgnoreMoveInput(true);
		GetController()->StopMovement();
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		SetActorLocation(spot->hidingPoint->GetComponentLocation());
		FirstPersonCameraComponent->SetRelativeLocation(cameraHidePosition);
		HeadBobComponent->SetCameraRelativeLocationStart(FirstPersonCameraComponent->GetRelativeLocation());

		if (InteractWidget)
		{
			InteractWidget->SetInteractMessage(FText::FromString("Press F to Leave"));
			InteractWidget->SetVisibility(ESlateVisibility::Visible);
		}

		if (GetWorld())
		{
			UAIDirectorSubsystem* Director = GetWorld()->GetSubsystem<UAIDirectorSubsystem>();
			if (Director)
			{
				Director->SetLoadCheckpointEnabled(false);
				Director->SetAIForgetPlayer();
			}
		}

		return true;
	} else {
		return false;
	}
}

void ASurvivorCharacter::Leave(AHidingSpot* spot) {
	if (Hidden) {
		GetController()->SetIgnoreMoveInput(false);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		SetActorLocation(spot->outPoint->GetComponentLocation());
		FirstPersonCameraComponent->SetRelativeLocation(cameraNormalPosition);
		HeadBobComponent->SetCameraRelativeLocationStart(FirstPersonCameraComponent->GetRelativeLocation());
		currentInteract = nullptr;

		if (InteractWidget)
		{
			InteractWidget->SetVisibility(ESlateVisibility::Hidden);
		}

		if (GetWorld())
		{
			UAIDirectorSubsystem* Director = GetWorld()->GetSubsystem<UAIDirectorSubsystem>();
			if (Director) Director->SetLoadCheckpointEnabled(true);
		}
	}
	Hidden = false;
}

bool ASurvivorCharacter::Trap(ATrap* trap) {
	if (!Trapped) {
		GetController()->SetIgnoreMoveInput(true);
		UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_None);
			SetActorLocation(trap->GetActorLocation() + FVector::UpVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		}
		currentInteract = (IInteractable*) trap;
		Trapped = true;
		if (InteractWidget)
		{
			InteractWidget->SetInteractMessage(FText::FromString(trap->GetInteractionMessage(this)));
			InteractWidget->SetVisibility(ESlateVisibility::Visible);
		}
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), this->GetActorLocation(), 1.0, this, 0.0f, FAIPerceptionTags::DistinctNoiseTag);

		if (GetWorld())
		{
			UAIDirectorSubsystem* Director = GetWorld()->GetSubsystem<UAIDirectorSubsystem>();
			if (Director) Director->SetLoadCheckpointEnabled(false);
		}

		return true;
	}
	else {
		return false;
	}
}

void ASurvivorCharacter::EscapeTrap(ATrap* trap) {
	if (Trapped) {
		GetController()->SetIgnoreMoveInput(false);
		UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
		}
		currentInteract = 0;
		Trapped = false;

		if (InteractWidget)
		{
			InteractWidget->SetVisibility(ESlateVisibility::Hidden);
		}

		if (GetWorld())
		{
			UAIDirectorSubsystem* Director = GetWorld()->GetSubsystem<UAIDirectorSubsystem>();
			if (Director) Director->SetLoadCheckpointEnabled(true);
		}
	}
}

void ASurvivorCharacter::Jump() {
	if (!Hidden && !Trapped) {
		Super::Jump();
		if (HeadBobComponent)
		{
			HeadBobComponent->PlayAdditiveCurve("Jump");
		}
	}
}

void ASurvivorCharacter::NotifyJumpApex()
{
	Super::NotifyJumpApex();
}

void ASurvivorCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if(HeadBobComponent)
		HeadBobComponent->PlayAdditiveCurve("Land");

	UAISense_Hearing::ReportNoiseEvent(GetWorld(), this->GetActorLocation(), 700.0F, this, 100.0F, FAIPerceptionTags::NoiseTag);
	if (FootStepSounds.Num() > 0)
	{
		USoundBase* FoostepSound = FootStepSounds[FMath::FRandRange(0, FootStepSounds.Num() - 1)];
		UGameplayStatics::PlaySound2D(GetWorld(), FoostepSound, 0.6F);
	}
}

void ASurvivorCharacter::Kill() {
	GetController()->SetIgnoreMoveInput(true);
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_None);
	}

	// in case there is no death screen widget
	EndInteract();
	if (GetWorld())
	{
		UAIDirectorSubsystem* Subsystem = GetWorld()->GetSubsystem<UAIDirectorSubsystem>();
		if (Subsystem)
		{
			Subsystem->SetLoadCheckpointEnabled(true);
		}
	}

	APlayerController* PlayerCont = Cast<APlayerController>(GetController());
	if (PlayerCont)
	{
		if (DeathWidget)
		{
			DeathWidget->SetVisibility(ESlateVisibility::Visible);
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerCont, DeathWidget, EMouseLockMode::DoNotLock);
			PlayerCont->bShowMouseCursor = true;
		}

		// this could be done a lot better, however not enough time
		TArray<AActor*> SoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAmbientSound::StaticClass(), SoundActors);
		for (AActor* SoundActor : SoundActors)
		{
			if (SoundActor->GetFName() == "ChaseMusic")
			{
				Cast<AAmbientSound>(SoundActor)->FadeOut(2.0F, 0.0F);
				break;
			}
		}
	}

	if (InteractWidget)
	{
		InteractWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Trapped)
	{
		currentInteract = 0;
		Trapped = false;
	}

	if (GetInteractingDialogueActor())
	{
		if (GetInteractingDialogueActor()->GetDialogueComponent()->ConversationIsRunning())
		{
			GetInteractingDialogueActor()->GetDialogueComponent()->ExitConversation();
		}
	}
}
