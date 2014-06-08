

#pragma once

#include "GameFramework/Actor.h"
#include "SurItem.generated.h"


UCLASS()
class ASurItem : public AActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
	TSubobjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, Category = Blueprint)
		FString BlueprintPath;

	virtual void BeginPlay() OVERRIDE;



public:

	TSubclassOf<class AActor> SurItemBlueprint;

	//  CONFIGURATION  ##################################################################

	UPROPERTY(EditDefaultsOnly, Category = Configuration)
		FName UIName;

	UPROPERTY(EditDefaultsOnly, Category = Configuration)
		UTexture* UITexture;

	UPROPERTY(EditDefaultsOnly, Category = Configuration)
		int32 CurrentItemCount;

	UPROPERTY(EditDefaultsOnly, Category = Configuration)
		int32 MaxStackable;





	//  INVENTORY  ##########################################################################

	// Handle self things when picked up.
	UFUNCTION(BlueprintCallable, Category = Inventory)
		virtual void ItemPickedUp();

	// Handle self things when dropped
	UFUNCTION(BlueprintCallable, Category = Inventory)
		virtual void ItemDropped(FVector Trajectory);


	UFUNCTION()
		virtual void OnItemEquipped();

	UFUNCTION()
		virtual void OnItemUnEquipped();

	UFUNCTION()
		virtual void OnUseItem();

protected:

	
};
