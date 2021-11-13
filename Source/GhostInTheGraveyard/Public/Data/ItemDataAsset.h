// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "ItemDataAsset.generated.h"

/**
 * The base data asset used to store item information.
 */
USTRUCT(Blueprintable)
struct GHOSTINTHEGRAVEYARD_API FItemDataAsset : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	// name of the item
	UPROPERTY(EditAnywhere)
	FName Name = "Item_Name";

	// the max capacity of having this item, -1 means infinite
	UPROPERTY(EditAnywhere)
	int32 MaxCapacity = 1;

	// icon texture
	UPROPERTY(EditAnywhere)
	UTexture2D* ItemIcon = nullptr;
};
