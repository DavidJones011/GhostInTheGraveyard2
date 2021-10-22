// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavModifierComponent.h"
#include "FixNavModifierComponent.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API UFixNavModifierComponent : public UNavModifierComponent
{
	GENERATED_BODY()

	virtual void CalcAndCacheBounds() const override;
	
};
