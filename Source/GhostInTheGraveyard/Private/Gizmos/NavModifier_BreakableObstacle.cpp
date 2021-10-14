// Fill out your copyright notice in the Description page of Project Settings.


#include "Gizmos/NavModifier_BreakableObstacle.h"
#include "NavAreas/NavArea_Default.h"
#include "Gizmos/NavArea_BreakableObstacle.h"

ANavModifier_BreakableObstacle::ANavModifier_BreakableObstacle(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCanEverAffectNavigation(false);
	SetAreaClass(UNavArea_BreakableObstacle::StaticClass());
	Tags.Add("Breakable_Obstacle");
}

float ANavModifier_BreakableObstacle::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	return DamageAmount;
}

void ANavModifier_BreakableObstacle::Break()
{
	SetAreaClass(UNavArea_Default::StaticClass());
}

void ANavModifier_BreakableObstacle::ResetBreakable()
{
	SetAreaClass(UNavArea_BreakableObstacle::StaticClass());
}
