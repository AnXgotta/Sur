

#pragma once

#include "Object.h"
#include "SurItem.h"
#include "SurInventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class USurInventorySlot : public UObject
{
	GENERATED_UCLASS_BODY()

	//  Max Items stackable in this slot
	UPROPERTY()
	int32 MaxStackableInSlot;


public:

	// Item Name
	UPROPERTY()
		FName ItemDisplayName;

	UFUNCTION(BlueprintCallable, Category = ItemSlot)
		FName GetItemName();

	// Number Stacked
	UPROPERTY()
		int32 NumberItemsStacked;

	TSubclassOf<class AActor> ItemBlueprint;


	//  ADD ITEM HELPERS  #####################################################################

	UFUNCTION(BlueprintCallable, Category=ItemSlot)
		bool IsSlotEmpty();

	



	//  ADD ITEM  ##############################################################################

	// Add item to inventory slot
	UFUNCTION(BlueprintCallable, Category = ItemSlot)
		int32 AddItemToSlot(ASurItem* NewItem, int32 Count);


	//  REMOVE ITEM  ##########################################################################

	// Remove item from inventory slot
	UFUNCTION(BlueprintCallable, Category = ItemSlot)
		void RemoveItemFromSlot(int32 Count);
	
};
