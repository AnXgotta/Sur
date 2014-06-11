

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

	UPROPERTY()
		TArray<USurInventorySlot* > ActionBar;

	// divisible by 8 and 4 -- unless dimensions change
	UPROPERTY()
	int32 MaxSize;


	//  STATUS  ####################################################

	UFUNCTION()
		bool IsFull();

	UFUNCTION()
		void Initialize();

	UFUNCTION()
		bool AddItemToInventoryFromItem(ASurItem* NewItem);

	UFUNCTION()
		bool AddItemToInventoryFromSlot(USurInventorySlot* NewItem);

	UFUNCTION()
		void RemoveItemFromInventory(int32 Index);

	UFUNCTION()
		bool AddItemToActionBarFromItem(ASurItem* NewItem);

	UFUNCTION()
		bool AddItemToActionBarFromSlot(USurInventorySlot* NewItemClot);

	UFUNCTION()
		void RemoveItemFromActionBar(int32 Index);


	//  HELPER METHODS  ##############################################

	UFUNCTION()
		USurInventorySlot* FindSlotForNewItem(ASurItem* NewItem);


};
