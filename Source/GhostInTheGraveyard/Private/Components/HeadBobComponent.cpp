// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HeadBobComponent.h"
#include "Curves/CurveVector.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UHeadBobComponent::UHeadBobComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UHeadBobComponent::BeginPlay()
{
	Super::BeginPlay();

	UCameraComponent* Camera = Cast<UCameraComponent>(GetOwner()->GetComponentByClass(UCameraComponent::StaticClass()));
	if (Camera)
	{
		CameraRelativeLocationStart = Camera->GetRelativeLocation();
	}
}


FVector UHeadBobComponent::StepVectorCurve(const UCurveVector* InCurve, float DeltaTime, bool bLoop /*= true*/)
{
	if(InCurve == NULL)
		return FVector::ZeroVector;

	float MinTime, MaxTime;
	InCurve->GetTimeRange(MinTime, MaxTime);

	BaseTimeAccumulator += DeltaTime;
	AddTimeAccumulator += DeltaTime;

	// update time accumulator with looping
	if (bLoop)
	{
		float ElapsedTime = MaxTime - MinTime;
		float LeftOverTime = 0.0;
		if (BaseTimeAccumulator > MaxTime)
		{
			LeftOverTime = BaseTimeAccumulator - MaxTime;
			BaseTimeAccumulator = MinTime + LeftOverTime;
		}
		else if (BaseTimeAccumulator < MinTime)
		{
			LeftOverTime = MinTime - BaseTimeAccumulator;
			BaseTimeAccumulator = MaxTime - LeftOverTime;
		}
	}
	else
	{
		BaseTimeAccumulator = FMath::Clamp<float>(AddTimeAccumulator, MinTime, MaxTime);
	}

	return InCurve->GetVectorValue(BaseTimeAccumulator);
}

// Called every frame
void UHeadBobComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UCameraComponent* Camera = Cast<UCameraComponent>(GetOwner()->GetComponentByClass(UCameraComponent::StaticClass()));

	// no camera found, leave early
	if(Camera == NULL)
		return;

	UCharacterMovementComponent* CharacterMovementComponent = Cast<ACharacter>(GetOwner())->GetCharacterMovement();
	FVector TargetRelativeLocation = FVector::ZeroVector;

	if (CharacterMovementComponent->GetLastUpdateVelocity().SizeSquared() > 0.0F && CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Walking)
	{
		TargetRelativeLocation = StepVectorCurve(WalkCurve, DeltaTime, true);
	}
	else
	{
		TargetRelativeLocation = StepVectorCurve(IdleCurve, DeltaTime, true);
	}

	if (bPlayingAdditiveCurve)
	{
		TargetRelativeLocation += StepVectorCurve(CurrentAdditiveCurve, DeltaTime, false);
	}

	Camera->SetRelativeLocation(CameraRelativeLocationStart + TargetRelativeLocation);
}

void UHeadBobComponent::PlayAdditiveCurve(const FName& Name)
{
	CurrentAdditiveCurve = *AdditiveCurves.Find(Name);
	if(!CurrentAdditiveCurve)
		return;

	float MinTime, MaxTime;
	CurrentAdditiveCurve->GetTimeRange(MinTime, MaxTime);
	const float Duration = MaxTime - MinTime;
	GetWorld()->GetTimerManager().SetTimer(AdditiveCurveTimerHandle, this, &UHeadBobComponent::EndAdditiveAnim, Duration);
	bPlayingAdditiveCurve = true;
	AddTimeAccumulator = MinTime;
}


