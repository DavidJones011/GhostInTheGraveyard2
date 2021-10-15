// Fill out your copyright notice in the Description page of Project Settings.


#include "HidingSpot.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "SurvivorCharacter.h"

// Sets default values
AHidingSpot::AHidingSpot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHidingSpot::BeginPlay()
{
	Super::BeginPlay();
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

// Called every frame
void AHidingSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

void AHidingSpot::Hide(ASurvivorCharacter* player) {
	player->SetActorLocation(hidingPoint.GetComponentLocation());
}

void AHidingSpot::UnHide(ASurvivorCharacter* player) {
	player->SetActorLocation(outPoint.GetComponentLocation());
}

