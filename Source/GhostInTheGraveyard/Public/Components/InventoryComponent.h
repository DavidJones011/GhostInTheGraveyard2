// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ItemDataAsset.h"
#include "InventoryComponent.generated.h"

class UDataTable;

USTRUCT()
struct GHOSTINTHEGRAVEYARD_API FInventorySlot
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 Count = 0;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GHOSTINTHEGRAVEYARD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	/**
	 * Adds the item to the inventory. Returns the number of items that cannot be carried.
	 */
	int32 AddItem(FName ItemName, int32 Count = 1);

	/**
	 * Removes the item from the inventory. Returns the actual number of items that were removed.
	 */
	int32 RemoveItem(FName ItemName, int32 Count = 1);

	/**
	 * Empties the inventory.
	 */
	void EmptyInventory();

	/**
	 * Get the item count in the inventory. Includes multiple of the same object.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetInventoryItemCount() const { return ItemCount; }

	/**
	 * Get the number of different items.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetInventoryDifferentItemCount() const { return DifferentItemCount; }

// 	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAddItemsSignature, const FItemDataAsset&, ItemData, int32, Count);
// 	FAddItemsSignature OnAddItem;
// 
// 	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRemoveItemsSignature, const FItemDataAsset&, ItemData, int32, Count);
// 	FRemoveItemsSignature OnRemoveItem;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UDataTable* ItemDatabase = nullptr;

private: 

	UPROPERTY()
	TMap<FName, FInventorySlot> Inventory;

	UPROPERTY()
	int32 ItemCount = 0;

	UPROPERTY()
	int32 DifferentItemCount = 0;
};
