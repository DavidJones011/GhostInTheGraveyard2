// Fill out your copyright notice in the Description page of Project Settings.


#include "Gizmos/BreakableObstacle.h"
#include "Gizmos//NavArea_BreakableObstacle.h"
#include "NavAreas/NavArea_Default.h"
#include "NavModifierComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ABreakableObstacle::ABreakableObstacle() : Super()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetBoxExtent(FVector(45,100,100), false);
	SetRootComponent(BoxComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCanEverAffectNavigation(false);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	NavModifierComponent = CreateDefaultSubobject<UNavModifierComponent>(TEXT("Nav Modifier Component"));
	NavModifierComponent->SetAreaClass(UNavArea_BreakableObstacle::StaticClass());
}

// Called when the game starts or when spawned
void ABreakableObstacle::BeginPlay()
{
	Super::BeginPlay();
}

void ABreakableObstacle::Break()
{
	NavModifierComponent->SetAreaClass(UNavArea_Default::StaticClass());
}

void ABreakableObstacle::Reset()
{
	NavModifierComponent->SetAreaClass(UNavArea_BreakableObstacle::StaticClass());
}

float ABreakableObstacle::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;

	if (Health <= 0)
	{
		Break();
	}

	return DamageAmount;
}

