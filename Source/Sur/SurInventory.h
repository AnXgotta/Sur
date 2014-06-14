

#pragma once

#include "Object.h"
#include "SurItem.h"
#include "SurInventorySlot.h"
#include "SurInventory.generated.h"

/**
 * 
 */
UCLASS()
class USurInventory : public UObject
{
	GENERATED_UCLASS_BODY()


	//  PROPERTIES  ###############################################

	UPROPERTY()
	TArray<USurInventorySlot* > Inventory;

	// divisible by 8 and 4 -- unless dimensions change
	UPROPERTY()
	int32 MaxSize;


	//  STATUS  ####################################################

	UFUNCTION()
		bool IsFull();

	UFUNCTION()
		void Initialize(int32 TotalSize);

	UFUNCTION()
		bool AddItemToInventoryFromItem(ASurItem* NewItem);

	UFUNCTION()
		bool AddItemToInventorySlotFromSlot(USurInventorySlot* FromSlot, USurInventorySlot* ToSlot);

	UFUNCTION()
		void RemoveItemFromInventory(int32 Index);

	UFUNCTION()
		void MoveItem(int32 FromIndex, int32 ToIndex);


	//  HELPER METHODS  ##############################################

	UFUNCTION()
		USurInventorySlot* FindSlotForNewItem(ASurItem* NewItem);


};
