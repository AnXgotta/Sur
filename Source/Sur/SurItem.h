

#pragma once

#include "GameFramework/Actor.h"
#include "SurItem.generated.h"


UENUM()
namespace EItemAction{
	enum Type{
		Null,
		Build,
		Consume,
		Fire,
		Swing
	};
}



UCLASS()
class ASurItem : public AActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = AAMesh)
	TSubobjectPtr<class UStaticMeshComponent> Mesh;


	virtual void BeginPlay() OVERRIDE;



public:
	UPROPERTY(EditDefaultsOnly, Category = AABlueprint)
	TSubclassOf<class ASurItem> SurItemBlueprint;

	//  CONFIGURATION  ##################################################################

	UPROPERTY(EditDefaultsOnly, Category = AAConfiguration)
		FName UIName;

	UPROPERTY(EditDefaultsOnly, Category = AAConfiguration)
		UTexture2D* UITexture;

	UPROPERTY(EditDefaultsOnly, Category = AAConfiguration)
		int32 CurrentItemCount;

	UPROPERTY(EditDefaultsOnly, Category = AAConfiguration)
		int32 MaxStackable;

	UPROPERTY(EditDefaultsOnly, Category = AAConfiguration)
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
