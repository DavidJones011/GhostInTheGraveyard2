// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Interactable.h"
#include "SurvivorCharacter.h"

#include "Trap.generated.h"


UCLASS()
class GHOSTINTHEGRAVEYARD_API ATrap : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATrap();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components");
	class USceneComponent* pivot;

	UPROPERTY(EditAnywhere, Category = "Components");
	class UStaticMeshComponent* hide;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components");
	class UBoxComponent* collider;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void Interact(ASurvivorCharacter* player);
	UFUNCTION(BlueprintCallable)
	virtual void EndInteract(ASurvivorCharacter* player);
	UFUNCTION(BlueprintCallable)
	virtual bool CanInteract(ASurvivorCharacter* player);
	UPROPERTY(BlueprintReadWrite);
	ASurvivorCharacter* trappedPlayer;


public:
	UPROPERTY(BlueprintReadWrite);
	float escapeProgress;
	UPROPERTY(BlueprintReadWrite);
	bool escaping;

};
