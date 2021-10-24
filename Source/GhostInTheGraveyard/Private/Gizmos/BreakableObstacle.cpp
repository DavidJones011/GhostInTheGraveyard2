// Fill out your copyright notice in the Description page of Project Settings.


#include "Gizmos/BreakableObstacle.h"
#include "NavAreas/NavArea_Default.h"
#include "Gizmos/NavArea_BreakableObstacle.h"
#include "Components/FixNavModifierComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ABreakableObstacle::ABreakableObstacle(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh Component");
	BaseMeshComponent->SetupAttachment(RootComponent);
	BaseMeshComponent->SetCanEverAffectNavigation(false);

	NavModComponent = CreateDefaultSubobject<UFixNavModifierComponent>("Nav Modifier Component");
	NavModComponent->bAutoRegister = true;

	NavBoundsBoxComponent = CreateDefaultSubobject<UBoxComponent>("Nav Modifier Bounds Box");
	NavBoundsBoxComponent->SetupAttachment(BaseMeshComponent);
	NavBoundsBoxComponent->SetCanEverAffectNavigation(true);

	BrokenSceneRoot = CreateDefaultSubobject<USceneComponent>("Broken Mesh(s) Root");
	BrokenSceneRoot->SetupAttachment(BaseMeshComponent);
	
	//setup camera shake
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> CameraShakeClass(TEXT("/Game/Effects/CameraShakes/BP_BarricadeCameraShake"));
	if (CameraShakeClass.Succeeded())
	{
		CameraShake = CameraShakeClass.Class;
	}
}

// Called when the game starts or when spawned
void ABreakableObstacle::BeginPlay()
{
	Super::BeginPlay();
}

void ABreakableObstacle::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform); 
	UpdateBrokenComponents();
	ResetBreakable();
}

void ABreakableObstacle::UpdateBrokenComponents()
{
	if (BrokenSceneRoot)
	{
	 	TArray<USceneComponent*> Components;
		BrokenSceneRoot->GetChildrenComponents(true, Components);
	 	int32 Num = BrokenSceneRoot->GetNumChildrenComponents();
	 
	 	BrokenMeshComponents.Empty();
	 	BrokenMeshComponents.Reserve(Num);
	
	 	for (USceneComponent* Comp : Components)
	 	{
	 		UStaticMeshComponent* SMComp = Cast<UStaticMeshComponent>(Comp);
	 		FScriptDelegate OnSleepDelegate;
	 		OnSleepDelegate.BindUFunction(this, "OnBrokenComponentSleep");
	 		SMComp->OnComponentSleep.Add(OnSleepDelegate);
	 		SMComp->GetBodyInstance()->bGenerateWakeEvents = true;
	 		SMComp->SetCollisionProfileName("Destructible");
	 		SMComp->SetGenerateOverlapEvents(false);
			SMComp->SetCanEverAffectNavigation(false);
			BrokenMeshComponents.Add(SMComp);
		}
	}
}

void ABreakableObstacle::OnBrokenComponentSleep(UPrimitiveComponent* SleepingComponent, FName BoneName)
{
	UMeshComponent* MeshComp = Cast<UMeshComponent>(SleepingComponent);
	if (MeshComp)
	{
		SetBrokenComponentActive(MeshComp, false, false);
	}
}

void ABreakableObstacle::AddImpulseBrokenComponents(FVector InVelocity, FVector Point, bool ScaledToBrokenMass /*= false*/)
{
	for (UStaticMeshComponent* Comp : BrokenMeshComponents)
	{
		FVector FinalImpulse = (ScaledToBrokenMass) ? InVelocity * Comp->GetMass() : InVelocity;
		Comp->AddImpulseAtLocation(FinalImpulse, Point);
	}	
}

void ABreakableObstacle::SetBrokenComponentActive(UMeshComponent* BrokenComp, bool bIsActive, bool bHide)
{
	if (BrokenComp)
	{
		if (bIsActive)
		{
			BrokenComp->SetCollisionProfileName("PhysicsActor");
			BrokenComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			BrokenComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);
			BrokenComp->SetSimulatePhysics(true);
		}
		else
		{
			BrokenComp->SetSimulatePhysics(false);
			BrokenComp->SetCollisionProfileName("NoCollision");
			BrokenComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		if (BrokenComp->bHiddenInGame != bHide)
		{
			BrokenComp->SetHiddenInGame(bHide);
		}
	}
}

void ABreakableObstacle::SetBrokenComponentsActive(bool bIsActive, bool bHide)
{
	for (UStaticMeshComponent* Comp : BrokenMeshComponents)
	{
		SetBrokenComponentActive(Comp, bIsActive, bHide);
	}
}

void ABreakableObstacle::ForceBrokenComponentsSleep()
{
	SetBrokenComponentsActive(false, true);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void ABreakableObstacle::Break()
{
	bBroken = true;
	BaseMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BaseMeshComponent->SetHiddenInGame(true);
	NavModComponent->SetAreaClass(UNavArea_Default::StaticClass());

	SetBrokenComponentsActive(true, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABreakableObstacle::ForceBrokenComponentsSleep, ForceSleepTime);
}

void ABreakableObstacle::ResetBreakable()
{
	bBroken = false;
	BaseMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BaseMeshComponent->SetHiddenInGame(false);
	NavModComponent->SetAreaClass(UNavArea_BreakableObstacle::StaticClass());
	

	Health = 100.0F;
	SetBrokenComponentsActive(false, true);
	for (UStaticMeshComponent* Comp : BrokenMeshComponents)
	{
		Comp->AttachToComponent(BrokenSceneRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Comp->SetRelativeLocationAndRotation(FVector::ZeroVector, FQuat::Identity);
	}
}

float ABreakableObstacle::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (!bBroken)
	{
		float NewHealth = Health - DamageAmount;
		float DamageDealt = DamageAmount;
		if (NewHealth < 0)
		{
			DamageDealt = Health;
			Health = 0;
		}
		else
		{
			Health -= DamageAmount;
		}

		if (CameraShake)
		{
			UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, GetActorLocation(), 0.0F, 600.0F, 0.5F);
		}

		if (FMath::IsNearlyZero(Health))
		{
			Break();
			
			AddImpulseBrokenComponents(DamageCauser->GetActorForwardVector() * 200.0F, DamageCauser->GetActorLocation(), true);		
		}

		return DamageDealt;
	}

	return 0;
}

