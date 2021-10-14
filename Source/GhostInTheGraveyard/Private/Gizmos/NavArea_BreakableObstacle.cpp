// Fill out your copyright notice in the Description page of Project Settings.


#include "Gizmos/NavArea_BreakableObstacle.h"

UNavArea_BreakableObstacle::UNavArea_BreakableObstacle(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DrawColor = FColor(255, 153, 51);
	FixedAreaEnteringCost = 2000.0F;
}

