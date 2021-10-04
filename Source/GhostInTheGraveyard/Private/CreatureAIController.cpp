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
				GetBlackboardComponent()->SetValueAsObject(FBBKeys::TargetActor, DetectedActor);
				GetBlackboardComponent()->ClearValue(FBBKeys::TargetLocation);
				GetBlackboardComponent()->ClearValue(FBBKeys::InvestigateState);
			}
		}
		else
		{
			if (TargetActor == DetectedActor && Stage == (uint32)EDetectionStage::Curious)
			{
				GetBlackboardComponent()->ClearValue(FBBKeys::TargetActor);
				GetBlackboardComponent()->SetValueAsVector(FBBKeys::TargetLocation, DetectedActor->GetActorLocation());
				GetBlackboardComponent()->SetValueAsEnum(FBBKeys::InvestigateState, EInvestigateState::Thorough_Search);
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

FVector ACreatureAIController::UpdateNextPatrolPoint()
{
	if (PatrolTrackerComponent)
	{
		return PatrolTrackerComponent->UpdateNextPatrolLocation();
	}
	return FVector(FLT_MAX); // this is how UE4 AI perceives invalid location
}

void ACreatureAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
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
}

