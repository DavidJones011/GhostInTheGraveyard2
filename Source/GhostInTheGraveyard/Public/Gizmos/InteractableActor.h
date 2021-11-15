// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InteractableActor.generated.h"

UCLASS()
class GHOSTINTHEGRAVEYARD_API AInteractableActor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Begin IInteractable interface */
	UFUNCTION(BlueprintCallable)
	virtual void Interact(ASurvivorCharacter* player) { OnInteract(player); };
	UFUNCTION(BlueprintCallable)
	virtual void EndInteract(ASurvivorCharacter* player) { OnEndInteract(player); };
	UFUNCTION(BlueprintCallable)
	virtual bool CanInteract(ASurvivorCharacter* player) { return OnCanInteract(player); }
	/* End IInteractable interface */

	UFUNCTION(BlueprintNativeEvent)
	void OnInteract(ASurvivorCharacter* player);
	virtual void OnInteract_Implementation(ASurvivorCharacter* player) {};

	UFUNCTION(BlueprintNativeEvent)
	void OnEndInteract(ASurvivorCharacter* player);
	virtual void OnEndInteract_Implementation(ASurvivorCharacter* player) {};

	UFUNCTION(BlueprintNativeEvent)
	bool OnCanInteract(ASurvivorCharacter* player);
	virtual bool OnCanInteract_Implementation(ASurvivorCharacter* player) { return true; }

};
