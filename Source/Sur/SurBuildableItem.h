

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





	// OVERLAP EVENTS

	UFUNCTION()
	void OnBeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnEndOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



	//  INTERACTION  #######################################################################

	
	UFUNCTION()
	void OnBeginBuilding();

	UFUNCTION()
	void OnEndBuilding(bool Cancelled);

	virtual void OnItemEquipped();

	virtual void OnItemUnEquipped();

	virtual void OnUseItem();
	
};
