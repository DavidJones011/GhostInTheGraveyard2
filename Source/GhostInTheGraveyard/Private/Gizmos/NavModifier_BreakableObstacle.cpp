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
	float NewHealth = Health - DamageAmount;
	float DamageDealt = DamageAmount;
	if (NewHealth < 0)
	{
		DamageDealt = Health;
		Health = 0;
	}
	else
	{
		Health -= DamageAmount;
	}

	if (FMath::IsNearlyZero(Health))
	{
		Break();
	}

	return DamageDealt;
}

void ANavModifier_BreakableObstacle::Break()
{
	SetAreaClass(UNavArea_Default::StaticClass());
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ANavModifier_BreakableObstacle::ResetBreakable()
{
	SetAreaClass(UNavArea_BreakableObstacle::StaticClass());
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
