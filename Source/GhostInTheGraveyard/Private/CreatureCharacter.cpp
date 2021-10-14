// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureCharacter.h"
#include "CreatureAIController.h"
#include "CollisionQueryParams.h"

// Sets default values
ACreatureCharacter::ACreatureCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = ACreatureAIController::StaticClass();

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
	const FVector TargetLocation = GetActorLocation() + GetActorForwardVector() * Distance;
	const ECollisionChannel DefaultSightCollisionChannel = ECollisionChannel::ECC_WorldDynamic;
	const bool bHit = GetWorld()->LineTraceSingleByChannel(OutResult, GetActorLocation(), TargetLocation
		, DefaultSightCollisionChannel
		, FCollisionQueryParams("", true, this));
	return bHit;
}

