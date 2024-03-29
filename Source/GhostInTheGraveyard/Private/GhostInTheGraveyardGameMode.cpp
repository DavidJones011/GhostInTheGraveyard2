// Copyright Epic Games, Inc. All Rights Reserved.

#include "GhostInTheGraveyardGameMode.h"
#include "GhostInTheGraveyardHUD.h"
#include "GhostInTheGraveyardCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGhostInTheGraveyardGameMode::AGhostInTheGraveyardGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AGhostInTheGraveyardHUD::StaticClass();
}
