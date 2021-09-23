// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnvironmentQuery/EnvQueryGenerator_ContextPoints.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "AI/AIPointContextManager.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

#define LOCTEXT_NAMESPACE "EnvQueryGenerator"

UEnvQueryGenerator_ContextPoints::UEnvQueryGenerator_ContextPoints(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemType = UEnvQueryItemType_Point::StaticClass();
	PointType = EPointType::Search;
	GenerateOnlyInRadius.DefaultValue = true;
	SearchRadius.DefaultValue = 500.0F;
	SearchCenter = UEnvQueryContext_Querier::StaticClass();
}

void UEnvQueryGenerator_ContextPoints::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (!QueryOwner)
	{
		return;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(QueryOwner, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return;
	}

	GenerateOnlyInRadius.BindData(QueryOwner, QueryInstance.QueryID);
	bool bUseRadius = (bool)GenerateOnlyInRadius.GetValue();

	TArray<FVector> MatchingPoints;
	if (bUseRadius)
	{
		TArray<FVector> ContextLocations;
		QueryInstance.PrepareContext(SearchCenter, ContextLocations);

		SearchRadius.BindData(QueryOwner, QueryInstance.QueryID);
		const float RadiusValue = SearchRadius.GetValue();
		const float RadiusSquared = FMath::Square(RadiusValue);

		// currently finds all AI point context managers and grabs all their points, could be optimized
		// theoretically there shouldn't be more than one manager
		for (TActorIterator<AAIPointContextManager> ItManager = TActorIterator<AAIPointContextManager>(World, AAIPointContextManager::StaticClass()); ItManager; ++ItManager)
		{
			TArray<FVector>& PointsOfInterest = *GetArrayOfPointType(*ItManager);
			for (int32 PointIndex = 0; PointIndex < PointsOfInterest.Num(); ++PointIndex)
			{
				FVector& Point = PointsOfInterest[PointIndex];
				for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ++ContextIndex)
				{
					if (FVector::DistSquared(ContextLocations[ContextIndex], Point) < RadiusSquared)
					{
						MatchingPoints.Add(Point);
						break;
					}
				}
			}
		}
	}
	else
	{
		for (TActorIterator<AAIPointContextManager> ItManager = TActorIterator<AAIPointContextManager>(World, AAIPointContextManager::StaticClass()); ItManager; ++ItManager)
		{
			TArray<FVector>& PointsOfInterest = *GetArrayOfPointType(*ItManager);
			for (int32 PointIndex = 0; PointIndex < PointsOfInterest.Num(); ++PointIndex)
			{
				MatchingPoints.Add(PointsOfInterest[PointIndex]);
			}
		}
	}

	ProcessItems(QueryInstance, MatchingPoints);
	QueryInstance.AddItemData<UEnvQueryItemType_Point>(MatchingPoints);
}

TArray<FVector>* UEnvQueryGenerator_ContextPoints::GetArrayOfPointType(AAIPointContextManager* Manager) const
{
	TArray<FVector>* PointsOfInterest = nullptr;
	switch (PointType)
	{
	case EPointType::Search:
		PointsOfInterest = &Manager->SearchPoints;
		break;
	case EPointType::SoundTransfer:
		PointsOfInterest = &Manager->SoundTransferPoints;
		break;
	case EPointType::Patrol:
		PointsOfInterest = &Manager->PatrolPoints;
		break;
	default:
		break;
	}

	return PointsOfInterest;
}

FText UEnvQueryGenerator_ContextPoints::GetDescriptionTitle() const
{
	FFormatNamedArguments Args;
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPointType"), true);		
	const FText PointTypeText = (EnumPtr) ? EnumPtr->GetDisplayNameTextByIndex(PointType) : FText::FromString("Invalid Type");
	Args.Add(TEXT("DescriptionTitle"), Super::GetDescriptionTitle());
	Args.Add(TEXT("PointType"), PointTypeText);

	if (!GenerateOnlyInRadius.IsDynamic() && !GenerateOnlyInRadius.GetValue())
	{
		return FText::Format(LOCTEXT("DescriptionGeneratePoints", "{DescriptionTitle}: generate set of points of the type {PointType}"), Args);
	}

	Args.Add(TEXT("DescribeContext"), UEnvQueryTypes::DescribeContext(SearchCenter));
	return FText::Format(LOCTEXT("DescriptionGeneratePointsAroundContext", "{DescriptionTitle}: generate set of points of type {PointType} around {DescribeContext}"), Args);
}

FText UEnvQueryGenerator_ContextPoints::GetDescriptionDetails() const
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("Radius"), FText::FromString(SearchRadius.ToString()));

	FText Desc = FText::Format(LOCTEXT("ContextPointsDescription", "radius: {Radius}"), Args);

	return Desc;
}

#undef LOCTEXT_NAMESPACE