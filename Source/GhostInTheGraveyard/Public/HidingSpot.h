// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "SurvivorCharacter.h"

#include "HidingSpot.generated.h"

UCLASS()
class GHOSTINTHEGRAVEYARD_API AHidingSpot : public AActor
{
	GENERATED_BODY()
		DECLARE_MULTICAST_DELEGATE_OneParam(FPlayerHid, FVector);
	
public:	
	// Sets default values for this actor's properties
	AHidingSpot();

	UPROPERTY(EditAnywhere, Category = "Components");
	class UStaticMeshComponent* hide;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components");
	class USphereComponent* hidingPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components");
	class USphereComponent* outPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components");
	class USceneComponent* pivot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components");
	class UBoxComponent* collider;

	FPlayerHid Player_OnHide;

private:
	bool PlayerHiding;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "HidingSpot")
	void ToggleHide(ASurvivorCharacter * player);

	void InteractHandler(ASurvivorCharacter* player);
};
