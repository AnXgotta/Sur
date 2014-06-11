

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


//  HELPERS  ###################################################################

bool USurInventorySlot::IsSlotEmpty(){
	return NumberItemsStacked <= 0 ? true : false;
}

bool USurInventorySlot::IsSlotFull(){
	return NumberItemsStacked == MaxStackableInSlot;
}

void USurInventorySlot::ClearSlotInformation(){
	NumberItemsStacked = 0;
	MaxStackableInSlot = -1;
	ItemDisplayName = TEXT("NULL");
	ItemBlueprint = NULL;
	ItemDisplayTexture = NULL;
}

void USurInventorySlot::SetSlotInformation(ASurItem* NewItem){
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
}

int32 USurInventorySlot::GetNumberItemsStacked(){
	return NumberItemsStacked;
}

int32 USurInventorySlot::SpaceRemaining(){
	return MaxStackableInSlot - NumberItemsStacked;
}



int32  USurInventorySlot::AddItemToSlot(ASurItem* NewItem){
	
	SetSlotInformation(NewItem);

	// check for overflow items on drag and drop stacking
	int32 OverflowItemCount = 0;
	if ((NumberItemsStacked + NewItem->CurrentItemCount) > MaxStackableInSlot){
		OverflowItemCount = FMath::Max((NumberItemsStacked + NewItem->CurrentItemCount) - MaxStackableInSlot, 0);
		NumberItemsStacked = MaxStackableInSlot;
	}else{
		NumberItemsStacked += NewItem->CurrentItemCount;
	}

	return OverflowItemCount;
}

void USurInventorySlot::RemoveItemFromSlot(int32 Count){
	--NumberItemsStacked;
	if (NumberItemsStacked <= 0){
		ClearSlotInformation();
	}
}


