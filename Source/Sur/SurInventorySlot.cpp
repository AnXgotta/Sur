

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

int32 USurInventorySlot::GetSlotPosition(){
	return SlotPosition;
}

UTexture2D* USurInventorySlot::GetItemDisplayTexture(){
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


void USurInventorySlot::SetSlotInformationFromSlot(USurInventorySlot* OtherSlot){
	ItemBlueprint = OtherSlot->ItemBlueprint;
	ItemDisplayName = OtherSlot->ItemDisplayName;
	ItemDisplayTexture = OtherSlot->ItemDisplayTexture;
	NumberItemsStacked = OtherSlot->NumberItemsStacked;
	SlotPosition = OtherSlot->SlotPosition;
	MaxStackableInSlot = OtherSlot->MaxStackableInSlot;
}

void USurInventorySlot::SwapSlotInformation(USurInventorySlot* OtherSlot){
	TSubclassOf<class AActor> tItemBP = OtherSlot->ItemBlueprint;
	FName tName = OtherSlot->ItemDisplayName;
	UTexture2D* tTexture = OtherSlot->ItemDisplayTexture;
	int32 tNumItemStacked = OtherSlot->NumberItemsStacked;
	int32 tSlotPosition = OtherSlot->SlotPosition;
	int32 tMaxStackable = OtherSlot->MaxStackableInSlot;

	OtherSlot->ItemBlueprint = ItemBlueprint;
	OtherSlot->ItemDisplayName = ItemDisplayName;
	OtherSlot->ItemDisplayTexture = ItemDisplayTexture;
	OtherSlot->NumberItemsStacked = NumberItemsStacked;
	OtherSlot->SlotPosition = SlotPosition;
	OtherSlot->MaxStackableInSlot = MaxStackableInSlot;

	ItemBlueprint = tItemBP;
	ItemDisplayName = tName;
	ItemDisplayTexture = tTexture;
	NumberItemsStacked = tNumItemStacked;
	SlotPosition = tSlotPosition;
	MaxStackableInSlot = tMaxStackable;
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
		NewItem->CurrentItemCount = OverflowItemCount;
	}else{
		NumberItemsStacked += NewItem->CurrentItemCount;
		NewItem->CurrentItemCount = 0;
	}

	return OverflowItemCount;
}

void USurInventorySlot::RemoveItemFromSlot(int32 Count){
	--NumberItemsStacked;
	if (NumberItemsStacked <= 0){
		ClearSlotInformation();
	}
}


