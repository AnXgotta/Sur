

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




	virtual void PostInitializeComponents() OVERRIDE;

	// OVERLAP EVENTS

	UPROPERTY()
		UMaterialInstanceDynamic* MI_ObjectMat;

	UFUNCTION()
		void SetBuildableColor();

	UFUNCTION()
	void OnActorBeginOverlap(class AActor* OtherActor);

	UFUNCTION()
		void OnActorEndOverlap(class AActor* OtherActor);



	//  INTERACTION  #######################################################################

	UPROPERTY()
		int32 OverlappedObjects;

	UPROPERTY()
		bool bHasBeenBuilt;

	UFUNCTION()
		bool HasBeenBuilt();
	
	UFUNCTION()
	void OnBeginBuilding();

	UFUNCTION()
	void OnEndBuilding(bool Cancelled);

	UFUNCTION()
		bool CanBuild();

	virtual void OnItemEquipped();

	virtual void OnItemUnEquipped();

	virtual void OnUseItem();
	
};
