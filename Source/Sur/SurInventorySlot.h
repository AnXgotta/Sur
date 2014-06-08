

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

	TSubclassOf<class AActor> ItemBlueprint;

public:

	// Item Name
	UPROPERTY()
		FName ItemDisplayName;

	UFUNCTION(BlueprintCallable, Category = ItemSlot)
		FName GetItemName();

	UFUNCTION(BlueprintCallable, Category = ItemSlot)
		UTexture* GetItemDisplayTexture();

	UPROPERTY()
		UTexture* ItemDisplayTexture;

	// Number Stacked
	UPROPERTY()
		int32 NumberItemsStacked;

	UPROPERTY()
		FVector2D SlotPosition;

	UFUNCTION(BlueprintCallable, Category = ItemSlot)
		FVector2D GetSlotPosition();
	


	//  HELPERS  #####################################################################

	UFUNCTION(BlueprintCallable, Category=ItemSlot)
		bool IsSlotEmpty();

	UFUNCTION(BlueprintCallable, Category = ItemSlot)
		bool IsSlotFull();

	UFUNCTION(BlueprintCallable, Category = ItemSlot)
		int32 GetNumberItemsStacked();

	UFUNCTION()
		int32 SpaceRemaining();

	UFUNCTION()
		void ClearSlotInformation();

	UFUNCTION()
		void SetSlotInformation(ASurItem* NewItem);


	//  ADD ITEM  ##############################################################################

	// Add item to inventory slot
	UFUNCTION(BlueprintCallable, Category = ItemSlot)
		int32 AddItemToSlot(ASurItem* NewItem);


	//  REMOVE ITEM  ##########################################################################

	// Remove item from inventory slot
	UFUNCTION(BlueprintCallable, Category = ItemSlot)
		void RemoveItemFromSlot(int32 Count);
	
};
