// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "SurvivorCharacter.h"
#include "Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

#include "HidingSpot.generated.h"

UCLASS()
class GHOSTINTHEGRAVEYARD_API AHidingSpot : public AActor, public IInteractable
{
	GENERATED_BODY()
	
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


private:
	ASurvivorCharacter* PlayerHiding;

public:	
	UFUNCTION(BlueprintCallable)
	void Interact(ASurvivorCharacter* player);
	UFUNCTION(BlueprintCallable)
	bool CanInteract(ASurvivorCharacter* player);
	UFUNCTION(BlueprintCallable)
	void EndInteract(ASurvivorCharacter* player);
};
