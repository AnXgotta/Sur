

#include "Sur.h"
#include "SurInventorySlot.h"


USurInventorySlot::USurInventorySlot(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	MaxStackableInSlot = -1;
	ItemDisplayName = TEXT("NULL");
	NumberItemsStacked = 0;
	ItemDisplayTexture = NULL;
	ItemBlueprint = NULL;

}


FName USurInventorySlot::GetItemName(){
	if (!IsSlotEmpty()){
		if (ItemDisplayName != TEXT("NULL")){
			return ItemDisplayName;
		}
	}
	return TEXT("");
}

FVector2D USurInventorySlot::GetSlotPosition(){
	return SlotPosition;
}

UTexture* USurInventorySlot::GetItemDisplayTexture(){
	return ItemDisplayTexture;
}


bool USurInventorySlot::IsSlotEmpty(){
	return NumberItemsStacked == 0 ? true : false;
}

bool USurInventorySlot::IsSlotFull(){
	return NumberItemsStacked == MaxStackableInSlot;
}


int32  USurInventorySlot::AddItemToSlot(ASurItem* NewItem){
	// if no name (empty slot) add name
	if (ItemDisplayName == TEXT("NULL")){
		ItemDisplayName = FName(NewItem->UIName);
	}

	if (ItemDisplayTexture == NULL){
		ItemDisplayTexture = NewItem->UITexture;
	}

	if (!ItemBlueprint){
		ItemBlueprint = NewItem->SurItemBlueprint;
	}

	if (MaxStackableInSlot < 0){
		MaxStackableInSlot = NewItem->MaxStackable;
	}

	// check for overflow items on drag and drop stacking
	int32 OverflowItemCount = 0;
	if ((NumberItemsStacked + NewItem->CurrentItemCount) > MaxStackableInSlot){
		OverflowItemCount = (NumberItemsStacked + NewItem->CurrentItemCount) - MaxStackableInSlot;
		NumberItemsStacked = MaxStackableInSlot;
	}
	else{
		NumberItemsStacked += NewItem->CurrentItemCount;
	}

	return OverflowItemCount;
}


void USurInventorySlot::RemoveItemFromSlot(int32 Count){
	--NumberItemsStacked;
	if (NumberItemsStacked <= 0){
		NumberItemsStacked = 0;
		MaxStackableInSlot = -1;
		ItemDisplayName = TEXT("NULL");
		ItemBlueprint = NULL;
		ItemDisplayTexture = NULL;
	}
}
