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

	collider->SetupAttachment(pivot);



	escaping = false;
	escapeProgress = 0;
	trappedPlayer = 0;
}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();
	collider->OnComponentBeginOverlap.AddDynamic(this, &ATrap::OnOverlapBegin);
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (escaping) {
		escapeProgress += DeltaTime;
		if (escapeProgress >= 5.0) {
			trappedPlayer->EscapeTrap(this);
			disabled = true;
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);
			SetActorTickEnabled(false);
		}
	}

}

void ATrap::Interact(ASurvivorCharacter* player) {
	if (trappedPlayer == player) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Escaping"));
		escaping = true;
	}
}
void ATrap::EndInteract(ASurvivorCharacter* player) {
	if (disabled) {
		Destroy();
	}
	else if (trappedPlayer == player) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Don't give up"));
		escaping = false;
		escapeProgress = 0;
	}
}
bool ATrap::CanInteract(ASurvivorCharacter* player) {
	return player == trappedPlayer;
}

void ATrap::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Trapped"));

	ASurvivorCharacter* player = Cast<ASurvivorCharacter>(OtherActor);

	if (player) {
		player->Trap(this);
		trappedPlayer = player;
		this->Trap(player);
	}
}

void ATrap::Trap_Implementation(ASurvivorCharacter* player) {

}
