

#include "Sur.h"
#include "SurInventory.h"


USurInventory::USurInventory(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{


	MaxSize = 32;
}



bool USurInventory::IsFull(){
	for (int i = 0; i < Inventory.Num(); i++){
		if (!Inventory[i]->IsSlotFull()){
			return false;
		}
	}
	return true;
}


void USurInventory::Initialize(int32 TotalSize){
	MaxSize = TotalSize;
	for (int i = 0; i < MaxSize; i++){
		USurInventorySlot* NewSlot = NewObject<USurInventorySlot>();
		NewSlot->SlotPosition = i;
		Inventory.Add(NewSlot);
	}
}


int32 USurInventory::AddItemToInventoryFromItemStack(ASurItem* NewItem){
	if (!NewItem){
		return -1;
	}

	// check if inventory is full
	if (IsFull()){
		return 1;
	}

	int32 OverflowItems = 0;
	do{

		// check if inventory is full
		if (IsFull()){
			return 1;
		}

		USurInventorySlot* InventorySlot = FindStackSlotForNewItem(NewItem);

		if (InventorySlot){
			OverflowItems = InventorySlot->AddItemToSlot(NewItem);			
		}else{
			return NewItem->CurrentItemCount;
		}

	} while (OverflowItems > 0);

	if (OverflowItems <= 0){
		NewItem->ItemPickedUp();
	}
	return OverflowItems;
}

int32 USurInventory::AddItemToInventoryFromItemOpen(ASurItem* NewItem){
	if (!NewItem){
		return -1;
	}

	// check if inventory is full
	if (IsFull()){
		return 1;
	}

	int32 OverflowItems = 0;
	do{

		// check if inventory is full
		if (IsFull()){
			return 1;
		}

		USurInventorySlot* InventorySlot = FindOpenSlotForNewItem(NewItem);

		if (InventorySlot){
			OverflowItems = InventorySlot->AddItemToSlot(NewItem);
		}else{
			return NewItem->CurrentItemCount;
		}

	} while (OverflowItems > 0);

	if (OverflowItems <= 0){
		NewItem->ItemPickedUp();
	}
	
	return OverflowItems;
}

bool USurInventory::AddItemToInventorySlotFromSlot(USurInventorySlot* FromSlot, USurInventorySlot* ToSlot){

	return false;
}


void USurInventory::RemoveItemFromInventory(int32 Index){
	Inventory[Index]->RemoveItemFromSlot(Inventory[Index]->NumberItemsStacked);
}


void USurInventory::MoveItem(int32 FromIndex, int32 ToIndex){
	if (!Inventory.IsValidIndex(FromIndex) || !Inventory.IsValidIndex(ToIndex)) return;



}



//  HELPER METHODS  #########################################################

USurInventorySlot* USurInventory::FindStackSlotForNewItem(ASurItem* NewItem){
	for (int i = 0; i < Inventory.Num(); i++){
		USurInventorySlot* CurrentSlot = Inventory[i];
		if (!CurrentSlot) continue;
		if (NewItem->SurItemBlueprint == CurrentSlot->ItemBlueprint && !CurrentSlot->IsSlotEmpty() && !CurrentSlot->IsSlotFull()){
			return CurrentSlot;
		}
	}

	return NULL;
}

USurInventorySlot* USurInventory::FindOpenSlotForNewItem(ASurItem* NewItem){

	for (int i = 0; i < Inventory.Num(); i++){
		USurInventorySlot* CurrentSlot = Inventory[i];
		if (!CurrentSlot) continue;
		if (CurrentSlot->IsSlotEmpty()){
			return CurrentSlot;
		}
	}
	return NULL;
}
