// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTask_ChangeMaxSpeed.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/Composites/BTComposite_SimpleParallel.h"

UBTTask_ChangeMaxSpeed::UBTTask_ChangeMaxSpeed(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Change Max Speed";
	Duration = 5.0F;
	StartSpeed = 500.0F;
	EndSpeed = 800.0F;
	SpeedSteps = 3;
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_ChangeMaxSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AController* Controller = Cast<AController>(OwnerComp.GetOwner());
	if (Controller)
	{
		ACharacter* Character = Cast<ACharacter>(Controller->GetCharacter());
		if (Character)
		{
			FBTChangeMaxSpeedMemory* MyMemory = (FBTChangeMaxSpeedMemory*)NodeMemory;
			MyMemory->CurrentTime = 0.0F;
			MyMemory->StepDeltaTime = Duration / SpeedSteps;
			MyMemory->Step = 0;

			Character->GetCharacterMovement()->MaxWalkSpeed = StartSpeed;

			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

uint16 UBTTask_ChangeMaxSpeed::GetInstanceMemorySize() const
{
	return sizeof(FBTChangeMaxSpeedMemory);
}

void UBTTask_ChangeMaxSpeed::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTChangeMaxSpeedMemory* MyMemory = (FBTChangeMaxSpeedMemory*)NodeMemory;
	MyMemory->CurrentTime += DeltaSeconds;

	AController* Controller = Cast<AController>(OwnerComp.GetOwner());
	if (Controller)
	{
		ACharacter* Character = Cast<ACharacter>(Controller->GetCharacter());
		if (Character)
		{
			if (!FMath::IsNearlyZero(Duration) && !FMath::IsNearlyZero(EndSpeed - StartSpeed))
			{
				float NextTimeStep = FMath::Min<float>(Duration, MyMemory->Step * MyMemory->StepDeltaTime);
				if (MyMemory->CurrentTime >= NextTimeStep)
				{
					MyMemory->Step++;
					Character->GetCharacterMovement()->MaxWalkSpeed = StartSpeed + (MyMemory->CurrentTime / Duration) * (EndSpeed - StartSpeed);
				}
			}
			else
			{
				Character->GetCharacterMovement()->MaxWalkSpeed = StartSpeed;
			}
		}
	}

	//check if the task is being run as a child task of a parallel composite node
	UBTComposite_SimpleParallel* ParallelNode = Cast<UBTComposite_SimpleParallel>(GetParentNode());
	uint16 MainTaskIndex = ParallelNode->GetChildExecutionIndex(0);
	if (ParallelNode && GetExecutionIndex() > MainTaskIndex)
	{
		return;
	}

	if (MyMemory->CurrentTime >= Duration)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
