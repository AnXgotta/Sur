

#include "Sur.h"
#include "SurInventorySlot.h"


USurInventorySlot::USurInventorySlot(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	MaxStackableInSlot = 50;
	ItemDisplayName = TEXT("NULL");
	NumberItemsStacked = 0;

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

bool USurInventorySlot::IsSlotEmpty(){
	return NumberItemsStacked == 0 ? true : false;
}


int32  USurInventorySlot::AddItemToSlot(ASurItem* NewItem, int32 Count){
	// if no name (empty slot) add name
	if (ItemDisplayName == TEXT("NULL")){
		ItemDisplayName = FName(NewItem->GetUIName());
	}

	if (!ItemBlueprint){
		ItemBlueprint = NewItem->SurItemBlueprint;
	}

	// check for overflow items on drag and drop stacking
	int32 OverflowItemCount = 0;
	if ((NumberItemsStacked + Count) > MaxStackableInSlot){
		OverflowItemCount = (NumberItemsStacked + Count) - MaxStackableInSlot;
		NumberItemsStacked = MaxStackableInSlot;
	}
	else{
		NumberItemsStacked += Count;
	}

	return OverflowItemCount;
}


void USurInventorySlot::RemoveItemFromSlot(int32 Count){
	--NumberItemsStacked;
	if (NumberItemsStacked <= 0){
		NumberItemsStacked = 0;
		ItemDisplayName = TEXT("NULL");
	}
}
