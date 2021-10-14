// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavModifierVolume.h"
#include "NavModifier_BreakableObstacle.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTINTHEGRAVEYARD_API ANavModifier_BreakableObstacle : public ANavModifierVolume
{
	GENERATED_BODY()

public:

	ANavModifier_BreakableObstacle(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	void SetHealth(float NewHealth) { Health = NewHealth; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:

	UFUNCTION(BlueprintCallable)
	void Break();

	UFUNCTION(BlueprintCallable)
	void ResetBreakable();

private:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Health = 100;
	
};
