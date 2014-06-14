

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


bool USurInventory::AddItemToInventoryFromItem(ASurItem* NewItem){
	if (!NewItem){
		PRINT_SCREEN(TEXT("USurInventory [AddItem] NewItem NULL"));
		return false;
	}

	// check if inventory is full
	if (IsFull()){
		PRINT_SCREEN(TEXT("USurInventory [AddItem] Inventory Full"));
		return false;
	}

	int32 OverflowItems = 0;
	do{

		// check if inventory is full
		if (IsFull()){
			PRINT_SCREEN(TEXT("NOTICE:  USurInventory [AddItem] Inventory Full Do-While"));
			return false;
		}

		USurInventorySlot* InventorySlot = FindSlotForNewItem(NewItem);

		if (InventorySlot){
			OverflowItems = InventorySlot->AddItemToSlot(NewItem);			
		}else{
			PRINT_SCREEN(TEXT("NOTICE:  USurInventory [AddItem] InventorySlot NULL?"));
			return false;
		}

	} while (OverflowItems > 0);

	NewItem->ItemPickedUp();
	return true;
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

USurInventorySlot* USurInventory::FindSlotForNewItem(ASurItem* NewItem){
	for (int i = 0; i < Inventory.Num(); i++){
		USurInventorySlot* CurrentSlot = Inventory[i];
		if (!CurrentSlot) continue;
		if (NewItem->SurItemBlueprint == CurrentSlot->ItemBlueprint && !CurrentSlot->IsSlotEmpty() && !CurrentSlot->IsSlotFull()){
			PRINT_SCREEN("SurInventory [FindSlotForNewItem] Stacked on Slot");
			return CurrentSlot;
		}
	}

	for (int i = 0; i < Inventory.Num(); i++){
		USurInventorySlot* CurrentSlot = Inventory[i];
		if (!CurrentSlot) continue;
		if (CurrentSlot->IsSlotEmpty()){
			PRINT_SCREEN("SurInventory [FindSlotForNewItem] Added to New Slot");
			return CurrentSlot;
		}
	}
	return NULL;
}


