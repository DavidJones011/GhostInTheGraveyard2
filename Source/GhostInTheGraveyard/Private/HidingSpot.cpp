// Fill out your copyright notice in the Description page of Project Settings.


#include "HidingSpot.h"


// Sets default values
AHidingSpot::AHidingSpot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	pivot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	pivot->SetupAttachment(RootComponent);

	collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	collider->SetupAttachment(pivot);

	hide = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HidingSpotModel"));
	hide->SetupAttachment(pivot);

	hidingPoint = CreateDefaultSubobject<USphereComponent>(TEXT("HidingPoint"));
	hidingPoint->InitSphereRadius(10.0f);
	hidingPoint->SetupAttachment(pivot);

	outPoint = CreateDefaultSubobject<USphereComponent>(TEXT("OutPoint"));
	outPoint->InitSphereRadius(10.0f);
	outPoint->SetupAttachment(pivot);

	PlayerHiding = false;
}

// Called when the game starts or when spawned
void AHidingSpot::BeginPlay()
{
	Super::BeginPlay();
}

void AHidingSpot::Interact(ASurvivorCharacter* player) {
	if (collider->IsOverlappingActor(player)) {
		this->ToggleHide(player);
	}
}
bool AHidingSpot::CanInteract(ASurvivorCharacter* player) {
	return true;
}

void AHidingSpot::ToggleHide(ASurvivorCharacter* player) {
		/*if (PlayerHiding) {
			player->SetActorLocation(outPoint->GetComponentLocation());
			player->GetController()->SetIgnoreMoveInput(false);

			PlayerHiding = false;

			Player_OnHide.Broadcast(hidingPoint->GetComponentLocation());
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("UnHiding!"));
			}
		} else {
			player->SetActorLocation(hidingPoint->GetComponentLocation());
			player->GetController()->SetIgnoreMoveInput(true);

			PlayerHiding = true;

			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hiding"));
			}
		}*/
}

