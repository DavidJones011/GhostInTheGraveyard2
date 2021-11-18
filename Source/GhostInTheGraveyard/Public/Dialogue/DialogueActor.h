// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "DialogueActor.generated.h"

class UDialogueComponent;

UCLASS()
class GHOSTINTHEGRAVEYARD_API ADialogueActor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADialogueActor();

private:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Dialogue", meta = (AllowPrivateAccess = "true"))
	UDialogueComponent* DialogueComponent = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void ExitedConversation(ASurvivorCharacter* CharacterInstigator);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	/* IInteractable interface */
	UFUNCTION(BlueprintCallable)
	void Interact(ASurvivorCharacter* player);
	UFUNCTION(BlueprintCallable)
	void EndInteract(ASurvivorCharacter* player);
	UFUNCTION(BlueprintCallable)
	bool CanInteract(ASurvivorCharacter* player) { return true; }
	/* IInteractable interface */

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UDialogueComponent* GetDialogueComponent() { return DialogueComponent; }
};
