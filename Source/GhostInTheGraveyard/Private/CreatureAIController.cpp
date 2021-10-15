// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "AI/AIStrings.h"
#include "Components/DetectorComponent.h"
#include "Components/PatrolTrackerComponent.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "AI/AIDirectorSubsystem.h"
#include "CreatureCharacter.h"
#include "GameFramework/Character.h"

ACreatureAIController::ACreatureAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Setup Behavior Tree (Brain)
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Component"));
	if (BehaviorTreeComponent)
	{
		static ConstructorHelpers::FObjectFinder<UBehaviorTree> Obj(FAIPaths::CreatureBehaviorTreePath);
		if (Obj.Succeeded())
		{
			BehaviorTree = Obj.Object;
		}
	}

	// Setup Perception Component
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	if (GetPerceptionComponent())
	{
		SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
		if (SightConfig)
		{
			SightConfig->SightRadius = 500.0F;
			SightConfig->PeripheralVisionAngleDegrees = 50.0F;
			SightConfig->LoseSightRadius = 800.0F;
			SightConfig->DetectionByAffiliation.bDetectEnemies = true;
			SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
			SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
			GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ACreatureAIController::OnTargetPerceptionUpdate);
			GetPerceptionComponent()->ConfigureSense(*SightConfig);
			GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
		}

		HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
		if (HearingConfig)
		{
			HearingConfig->HearingRange = 3000.0F;
			HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
			HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
			HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
			GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ACreatureAIController::OnPerceptionUpdate);
			GetPerceptionComponent()->ConfigureSense(*HearingConfig);
		}
	}

	//Create the detector component
	DetectorComponent = CreateDefaultSubobject<UDetectorComponent>(TEXT("Detector Component"));
	DetectorComponent->OnStageChanged.BindUFunction(this, "OnDetectedUpdate");

	//Create patrol tracker component
	PatrolTrackerComponent = CreateDefaultSubobject<UPatrolTrackerComponent>(TEXT("Patrol Tracker Component"));
}

void ACreatureAIController::Tick(float DeltaTime)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (BlackboardComponent && BlackboardComponent->GetValueAsObject(FBBKeys::FoundBreakable) == NULL)
	{
		ACreatureCharacter* AICharacter = Cast<ACreatureCharacter>(GetCharacter());
		if(AICharacter)
		{
			FHitResult Result;
			if (AICharacter->CheckForActorInFront(150.0F, Result))
			{
				if (Result.GetActor()->ActorHasTag(FAITags::BreakableTag))
				{
					BlackboardComponent->SetValueAsObject(FBBKeys::FoundBreakable, Result.GetActor());
				}
			}
		}
	}
}

void ACreatureAIController::OnDetectedUpdate(AActor* DetectedActor, uint32 Stage)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (ensure(BlackboardComponent))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(FBBKeys::TargetActor));
		// no current target, just assign the target
		if (TargetActor == NULL)
		{
			if (Stage == (uint32)EDetectionStage::Aware)
			{
				GetBlackboardComponent()->SetValueAsBool(FBBKeys::PlayerSeen, true);
				GetBlackboardComponent()->SetValueAsEnum(FBBKeys::ActiveState, ECreatureState::ST_Pursue);
				GetBlackboardComponent()->SetValueAsObject(FBBKeys::TargetActor, DetectedActor);
				GetBlackboardComponent()->ClearValue(FBBKeys::TargetLocation);
				//GetBlackboardComponent()->ClearValue(FBBKeys::InvestigateState);
			}
		}
		else
		{
			if (TargetActor == DetectedActor && Stage == (uint32)EDetectionStage::Curious)
			{
				GetBlackboardComponent()->SetValueAsBool(FBBKeys::PlayerSeen, false);
				//GetBlackboardComponent()->SetValueAsBool(FBBKeys::ActiveState, ECreatureState::ST_Search);
				GetBlackboardComponent()->SetValueAsEnum(FBBKeys::ActiveState, ECreatureState::ST_Patrol);

				GetBlackboardComponent()->ClearValue(FBBKeys::TargetActor);
				GetBlackboardComponent()->SetValueAsVector(FBBKeys::TargetLocation, DetectedActor->GetActorLocation());
				//GetBlackboardComponent()->SetValueAsEnum(FBBKeys::InvestigateState, EInvestigateState::Thorough_Search);
			}
		}
	}
}

void ACreatureAIController::GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const
{
	if (GetCharacter())
	{
		out_Location = GetCharacter()->GetActorLocation();
		out_Rotation = GetCharacter()->GetActorRotation().GetEquivalentRotator();
	}
}

void ACreatureAIController::ReportEQSQueryResult(TSharedPtr<FEnvQueryResult> Result)
{
	FOccluderVertexArray ResultLocations;
	Result->GetAllAsLocations(ResultLocations);
}

bool ACreatureAIController::SendAIToPatrolPoint(AAIPointContextManager* Manager, int32 Section, int32 Index, bool bTeleport /*= false*/)
{
	if (PatrolTrackerComponent)
	{
		ACharacter* AICharacter = GetCharacter();
		bool bSuccess = (AICharacter != nullptr) && PatrolTrackerComponent->SetTrackedPatrolPoint(Manager, Section, Index);
		
		FVector Location = AICharacter->GetActorLocation();
		if (bSuccess && bTeleport)
		{
			Location = UpdateNextPatrolPoint();
			GetCharacter()->SetActorLocation(Location);
		}

		UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
		if (BlackboardComp)
		{
			if (BlackboardComp->GetValueAsEnum(FBBKeys::ActiveState) == ECreatureState::ST_Patrol)
			{
				BlackboardComp->SetValueAsVector(FBBKeys::TargetLocation, Location);
			}
		}

		return true;
	}
	return false;
}

bool ACreatureAIController::SendAIToPatrolSection(AAIPointContextManager* Manager, int32 Section, bool bTeleport /*= false*/)
{
	if (PatrolTrackerComponent)
	{
		if (Section == PatrolTrackerComponent->GetTrackedPatrolSection())
		{
			return false;
		}

		ACharacter* AICharacter = GetCharacter();
		bool bSuccess = (AICharacter != nullptr) && PatrolTrackerComponent->SetTrackedPatrolSection(Manager, Section);
		
		FVector Location = AICharacter->GetActorLocation();
		if (bSuccess && bTeleport)
		{
			Location = UpdateNextPatrolPoint();
			GetCharacter()->SetActorLocation(Location);
		}

		UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
		if (BlackboardComp)
		{
			if (BlackboardComp->GetValueAsEnum(FBBKeys::ActiveState) == ECreatureState::ST_Patrol)
			{
				BlackboardComp->SetValueAsVector(FBBKeys::TargetLocation, Location);
			}
		}

		return true;
	}
	return false;
}

FVector ACreatureAIController::UpdateNextPatrolPoint()
{
	if (PatrolTrackerComponent)
	{
		return PatrolTrackerComponent->UpdateNextPatrolLocation();
	}
	return FVector(FLT_MAX); // this is how UE4 AI perceives invalid location
}

void ACreatureAIController::InstantlyDetect(AActor* InActor)
{
	ACharacter* TargetCharacter = Cast<ACharacter>(InActor);
	if (TargetCharacter && DetectorComponent)
	{
		DetectorComponent->InstantlyDetectActor(InActor);
	}
}

void ACreatureAIController::InvestigateLocation(FVector Location)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();

	if(BlackboardComponent == nullptr)
		return;

	if (BlackboardComponent->GetValueAsBool(FBBKeys::PlayerSeen) == false) // don't care if the player is seen
	{
		BlackboardComponent->SetValueAsEnum(FBBKeys::ActiveState, ECreatureState::ST_Investigate);
		BlackboardComponent->SetValueAsVector(FBBKeys::TargetLocation, Location);
	}
}

void ACreatureAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}

	UWorld* World = GetWorld();
	if (World)
	{
		UAIDirectorSubsystem* Director = GetWorld()->GetSubsystem<UAIDirectorSubsystem>();
		Director->RegisterAIController(this);
	}
}

void ACreatureAIController::OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{
	// update other senses like hearing
}

void ACreatureAIController::OnTargetPerceptionUpdate(AActor* InActor, const FAIStimulus Stimulus)
{
	ACharacter* TargetCharacter = Cast<ACharacter>(InActor);
	if (TargetCharacter)
	{
		float Visibility = (Stimulus.WasSuccessfullySensed()) ? Stimulus.Strength : 0.0F;
		DetectorComponent->SetDetectedActor(InActor, Visibility);
	}

	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (BlackboardComponent && Stimulus.WasSuccessfullySensed())
	{
		if (BlackboardComponent->GetValueAsBool(FBBKeys::PlayerSeen) == false) // don't care if the player is seen
		{
			if (Stimulus.Tag == FAIPerceptionTags::DistinctNoiseTag)
			{
				BlackboardComponent->SetValueAsEnum(FBBKeys::ActiveState, ECreatureState::ST_Investigate);
				BlackboardComponent->SetValueAsVector(FBBKeys::TargetLocation, Stimulus.StimulusLocation);
			}
		}
	}
}

