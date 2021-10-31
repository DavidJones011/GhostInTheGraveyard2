// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Engine/DataTable.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	//setup default item database
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableObject(TEXT("/Game/Data/ItemDataTable"));
	if (ItemDataTableObject.Succeeded())
	{
		ItemDatabase = ItemDataTableObject.Object;
	}
}

int32 UInventoryComponent::AddItem(FName ItemName, int32 Count /* = 1*/)
{
	if (!ItemDatabase)
	{
		UE_LOG(LogTemp, Warning, TEXT("There was no Item Database set."));
		return Count;
	}

	FItemDataAsset* ItemData = ItemDatabase->FindRow<FItemDataAsset>(ItemName, FString("Finding item"));
	if (ItemData)
	{
		FInventorySlot& InventorySlot = Inventory.FindOrAdd(ItemName);
		bool bNew = InventorySlot.Count == 0;

		// account for not being able to grab all items
		int32 AdjustedCount = Count;
		if (InventorySlot.Count + Count > ItemData->MaxCapacity)
		{
			AdjustedCount = ItemData->MaxCapacity - InventorySlot.Count;
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0F, FColor::Blue, FString::Printf(TEXT("Added %s x%d times."), &ItemName, AdjustedCount));
		}

		InventorySlot.Count += AdjustedCount;
		ItemCount += AdjustedCount;
		if(bNew) DifferentItemCount++;
		return (Count - AdjustedCount);
	}

	return Count;
}

int32 UInventoryComponent::RemoveItem(FName ItemName, int32 Count /*= 1*/)
{
	FInventorySlot* InventorySlot = Inventory.Find(ItemName);

	if (!InventorySlot)
		return 0;

	int32 AdjustedRemoveCount =  Count;
	if (InventorySlot->Count - Count < 0)
	{
		AdjustedRemoveCount = InventorySlot->Count;
		DifferentItemCount--;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0F, FColor::Red, FString::Printf(TEXT("Removed %s x%d times."), &ItemName, AdjustedRemoveCount));
	}

	if(InventorySlot->Count == AdjustedRemoveCount)
		Inventory.Remove(ItemName);

	ItemCount -= AdjustedRemoveCount;
	return AdjustedRemoveCount;
}

void UInventoryComponent::EmptyInventory()
{
	Inventory.Empty();
	ItemCount = 0;
	DifferentItemCount = 0;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

