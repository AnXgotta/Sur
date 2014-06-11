

#pragma once

#include "GameFramework/Actor.h"
#include "SurItem.generated.h"


UENUM()
namespace EItemAction{
	enum Type{
		Null,
		Build,
		Fire,
		Swing
	};
}



UCLASS()
class ASurItem : public AActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
	TSubobjectPtr<class UStaticMeshComponent> Mesh;


	virtual void BeginPlay() OVERRIDE;



public:
	UPROPERTY(EditDefaultsOnly, Category = Blueprint)
	TSubclassOf<class ASurItem> SurItemBlueprint;

	//  CONFIGURATION  ##################################################################

	UPROPERTY(EditDefaultsOnly, Category = Configuration)
		FName UIName;

	UPROPERTY(EditDefaultsOnly, Category = Configuration)
		UTexture* UITexture;

	UPROPERTY(EditDefaultsOnly, Category = Configuration)
		int32 CurrentItemCount;

	UPROPERTY(EditDefaultsOnly, Category = Configuration)
		int32 MaxStackable;

	UPROPERTY(EditDefaultsOnly, Category = Configuration)
		TEnumAsByte<EItemAction::Type> ItemActionType;


	//  HELPERS  #############################################################################

	UFUNCTION()
		TEnumAsByte<EItemAction::Type> GetItemActionType();


	//  INVENTORY  ##########################################################################

	// Handle self things when picked up.
	UFUNCTION(BlueprintCallable, Category = Inventory)
		virtual void ItemPickedUp();

	// Handle self things when dropped
	UFUNCTION(BlueprintCallable, Category = Inventory)
		virtual void ItemDropped(FVector Trajectory);


	//  INTERACTION  #######################################################################

	UFUNCTION()
		virtual void OnItemEquipped();

	UFUNCTION()
		virtual void OnItemUnEquipped();

	UFUNCTION()
		virtual void OnUseItem();

protected:

	
};
