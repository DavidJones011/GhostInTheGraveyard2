// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "DataProviders/AIDataProvider.h"
#include "AI/AIPointContextManager.h"
#include "EnvQueryGenerator_ContextPoints.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "AI Context Points Of Type"))
class GHOSTINTHEGRAVEYARD_API UEnvQueryGenerator_ContextPoints : public UEnvQueryGenerator
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditDefaultsOnly, Category=Generator)
	TEnumAsByte<EPointType> PointType;
	
	/** If true, this will only returns points of the specified type within the SearchRadius of the SearchCenter context.  If false, it will return ALL points of the specified type. */
	UPROPERTY(EditDefaultsOnly, Category=Generator)
	FAIDataProviderFloatValue GenerateOnlyInRadius;

	/** Max distance of path between point and context.  NOTE: Zero and negative values will never return any results if
	* UseRadius is true.  "Within" requires Distance < Radius.  Actors ON the circle (Distance == Radius) are excluded.
	*/
	UPROPERTY(EditDefaultsOnly, Category = Generator)
	FAIDataProviderFloatValue SearchRadius;

	/** context */
	UPROPERTY(EditAnywhere, Category = Generator)
	TSubclassOf<UEnvQueryContext> SearchCenter;

	TArray<FVector>* GetArrayOfPointType(AAIPointContextManager* Manager) const;

	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;
	virtual void ProcessItems(FEnvQueryInstance& QueryInstance, TArray<FVector>& MatchingActors) const {}

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
};
