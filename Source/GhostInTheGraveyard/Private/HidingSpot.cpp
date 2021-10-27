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

	PlayerHiding = 0;
}

void AHidingSpot::Interact(ASurvivorCharacter* player) {
	if (PlayerHiding == player) {
		player->Leave(this);
	}
	else {
		if (player->Hide(this)) {
			PlayerHiding = player;
		}
	}
}

void AHidingSpot::EndInteract(ASurvivorCharacter* player) {
}
bool AHidingSpot::CanInteract(ASurvivorCharacter* player) {
	return true;
}

