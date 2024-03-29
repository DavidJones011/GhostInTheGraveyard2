// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Interactable.h"
#include "SurvivorCharacter.h"
#include "Trap.generated.h"

class USoundBase;

UCLASS()
class GHOSTINTHEGRAVEYARD_API ATrap : public AActor, public IInteractable
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	float TimeToDisableTrap = 2.0F;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> TrapCamShake;

	UPROPERTY(EditAnywhere)
	USoundBase* TrapSound;

	UPROPERTY(EditAnywhere)
	USoundBase* DisableSound;

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
	void Interact(ASurvivorCharacter* player);
	UFUNCTION(BlueprintCallable)
	void EndInteract(ASurvivorCharacter* player);
	UFUNCTION(BlueprintCallable)
	virtual bool CanInteract(ASurvivorCharacter* player);

	UFUNCTION(BlueprintCallable)
	virtual FString GetInteractionMessage(ASurvivorCharacter* player);

	UPROPERTY(BlueprintReadWrite);
	ASurvivorCharacter* trappedPlayer;

	UFUNCTION(BlueprintNativeEvent)
	void Trap(ASurvivorCharacter* player);

	UFUNCTION(BlueprintCallable)
	void EngageTrap(ASurvivorCharacter* player) {Trap(player);}

	UFUNCTION(BlueprintNativeEvent)
	void ResetTrap();
	virtual void ResetTrap_Implementation(){ trappedPlayer = nullptr; }


public:
	UPROPERTY(BlueprintReadWrite);
	float escapeProgress;
	UPROPERTY(BlueprintReadWrite);
	bool escaping;

	bool disabled;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
