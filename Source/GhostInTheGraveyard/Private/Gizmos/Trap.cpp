// Fill out your copyright notice in the Description page of Project Settings.


#include "Gizmos/Trap.h"
#include "Components/BoxComponent.h"

// Sets default values
ATrap::ATrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	pivot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	pivot->SetupAttachment(RootComponent);

	collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	collider->OnComponentBeginOverlap.AddDynamic(this, &ATrap::OnBoxBeginOverlap);
	
	collider->SetupAttachment(pivot);


	
	escaping = false;
	escapeProgress = 0;
	trappedPlayer = 0;
}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (escaping) {
		escapeProgress += DeltaTime;
		if (escapeProgress >= 5.0) {
			trappedPlayer->EscapeTrap(this);
			trappedPlayer = 0;
		}
	}
	
}

void ATrap::Interact(ASurvivorCharacter* player) {
	if (trappedPlayer == player) {
		escaping = true;
	}
}
void ATrap::EndInteract(ASurvivorCharacter* player) {
	if (trappedPlayer == player) {
		escaping = false;
		escapeProgress = 0;
	}
}
bool ATrap::CanInteract(ASurvivorCharacter* player) {
	return player == trappedPlayer;
}

void ATrap::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ASurvivorCharacter* character = Cast<ASurvivorCharacter>(OtherActor);

	if (character) {
		character->Trap(this);
	}
}
