

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
		void Initialize();

	UFUNCTION()
		bool AddItem(ASurItem* NewItem);

	UFUNCTION()
		void RemoveItem(USurInventorySlot* SurInventorySlot);


	//  HELPER METHODS  ##############################################

	UFUNCTION()
		USurInventorySlot* FindSlotForNewItem(ASurItem* NewItem);


};
