

#pragma once

#include "SurItem.h"
#include "SurBuildableItem.generated.h"

/**
 * 
 */
UCLASS()
class ASurBuildableItem : public ASurItem
{
	GENERATED_UCLASS_BODY()










	//  INTERACTION  #######################################################################

	
	virtual void OnItemEquipped();

	virtual void OnItemUnEquipped();

	virtual void OnUseItem();
	
};
