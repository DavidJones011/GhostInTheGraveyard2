// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GhostInTheGraveyardHUD.generated.h"

UCLASS()
class AGhostInTheGraveyardHUD : public AHUD
{
	GENERATED_BODY()

public:
	AGhostInTheGraveyardHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

