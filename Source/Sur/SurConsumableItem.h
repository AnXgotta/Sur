

#pragma once

#include "SurTypes.h"
#include "SurItem.h"
#include "SurConsumableItem.generated.h"

/**
 * 
 */
UCLASS()
class ASurConsumableItem : public ASurItem
{
	GENERATED_UCLASS_BODY()


	UPROPERTY(EditDefaultsOnly, Category = AAConsumableData)
	FConsumableItemData ConsumableData;




	//  INTERACTION  #######################################################################


	virtual void OnItemEquipped();

	virtual void OnItemUnEquipped();

	virtual void OnUseItem();
	
};
