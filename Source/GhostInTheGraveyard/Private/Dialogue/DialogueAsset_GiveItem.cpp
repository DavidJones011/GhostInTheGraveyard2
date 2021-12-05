// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue/DialogueAsset_GiveItem.h"
#include "Dialogue/GhostActor.h"
#include "SurvivorCharacter.h"
#include "Components/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

void UDialogueAsset_GiveItem::Execute(AActor* ThisActor, AActor* Instigator)
{

	ASurvivorCharacter* Survivor = Cast<ASurvivorCharacter>(Instigator);
	if(!Survivor)
		return;

	UInventoryComponent* Inventory = Survivor->GetInventoryComponent();
	if(!Inventory)
		return;

	Inventory->AddItem(ItemName);

	if(ItemObtainedSound) UGameplayStatics::PlaySound2D(Instigator, ItemObtainedSound);
}
