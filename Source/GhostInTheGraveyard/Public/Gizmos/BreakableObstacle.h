// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakableObstacle.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UFixNavModifierComponent;
class UBoxComponent;

UCLASS()
class GHOSTINTHEGRAVEYARD_API ABreakableObstacle : public AActor
{
	GENERATED_BODY()

private:

	FTimerHandle TimerHandle;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMeshComponent = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	USceneComponent* BrokenSceneRoot = nullptr;

	//Array of all broken parts
	UPROPERTY(VisibleAnywhere)
	TArray<UStaticMeshComponent*> BrokenMeshComponents;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UFixNavModifierComponent* NavModComponent = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* NavBoundsBoxComponent = nullptr;

protected:

	UPROPERTY(EditAnywhere)
	float ForceSleepTime = 0.0F;

	UPROPERTY(EditAnywhere)
	float Health = 100.F;

	UPROPERTY(VisibleAnywhere)
	bool bBroken = false;

public:	
	// Sets default values for this actor's properties
	ABreakableObstacle(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// called when child blueprints are compiled
	virtual void OnConstruction(const FTransform& Transform);

	UFUNCTION()
	void UpdateBrokenComponents();

	UFUNCTION()
	virtual void OnBrokenComponentSleep(UPrimitiveComponent* SleepingComponent, FName BoneName);

	UFUNCTION()
	void AddImpulseBrokenComponents(FVector InVelocity, FVector Point, bool ScaledToBrokenMass = false);

	UFUNCTION()
	void SetBrokenComponentActive(UMeshComponent* BrokenComp, bool bIsActive, bool bHide);

	UFUNCTION()
	void SetBrokenComponentsActive(bool bIsActive, bool bHide);

	UFUNCTION()
	void ForceBrokenComponentsSleep();

	UFUNCTION(BlueprintCallable)
	void Break();

	UFUNCTION(BlueprintCallable)
	void ResetBreakable();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsBroken() const { return bBroken; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetStaticMeshComponent() const { return BaseMeshComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealth() const { return Health; }
};
