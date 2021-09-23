
#pragma once

#include "Engine/TargetPoint.h"
#include "AIPointContextManager.generated.h"

// type of points to add
UENUM()
enum EPointType
{
	Search = 0,
	SoundTransfer = 1,
	Patrol = 2
};

UCLASS()
class GHOSTINTHEGRAVEYARD_API AAIPointContextManager : public ATargetPoint
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Points")
	TArray<FVector> SearchPoints;

	UPROPERTY(EditAnywhere, Category = "Points")
	TArray<FVector> SoundTransferPoints;

	UPROPERTY(EditAnywhere, Category = "Points")
	TArray<FVector> PatrolPoints;
};