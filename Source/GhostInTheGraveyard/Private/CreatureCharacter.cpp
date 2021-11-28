// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureCharacter.h"
#include "CreatureAIController.h"
#include "CollisionQueryParams.h"
#include "Components/CapsuleComponent.h"
#include "Components/DetectorComponent.h"
#include "AudioDevice.h"

// Sets default values
ACreatureCharacter::ACreatureCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = ACreatureAIController::StaticClass();

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	if (AudioComponent)
	{
		AudioComponent->SetupAttachment(RootComponent);
		AudioComponent->Activate();
		AudioComponent->bAutoDestroy = false;
	}
}

// Called when the game starts or when spawned
void ACreatureCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ACreatureCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACreatureCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ACreatureCharacter::CheckForActorInFront(float Distance, FHitResult& OutResult)
{
	const FVector TargetLocation = GetActorLocation() + GetVelocity().GetSafeNormal() * Distance;
	const ECollisionChannel DefaultSightCollisionChannel = ECollisionChannel::ECC_WorldStatic;

	const bool bHit = GetWorld()->SweepSingleByChannel(OutResult, GetActorLocation(), TargetLocation, FQuat::Identity
		, DefaultSightCollisionChannel
		, GetCapsuleComponent()->GetCollisionShape()
		, FCollisionQueryParams("", true, this));

	return bHit;
}

