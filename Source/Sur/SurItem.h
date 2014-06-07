

#pragma once

#include "GameFramework/Actor.h"
#include "SurItem.generated.h"


USTRUCT()
struct FItemDataUI{

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Name)
	FName UIName;

	//	UPROPERTY(EditDefaultsOnly, Category = Texture)
	//		Texture UIImage;

	UPROPERTY(EditDefaultsOnly, Category = Stack)
		int32 MaxStackable;

	// defaults
	FItemDataUI(){
		UIName = TEXT("Change Me in BP");
		//UIImage = null;
		MaxStackable = 0;

	}
};

UCLASS()
class ASurItem : public AActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
	TSubobjectPtr<class UStaticMeshComponent> Mesh;


	virtual void BeginPlay() OVERRIDE;



public:

	// getter for UI Data
	UFUNCTION(BlueprintCallable, Category = Item)
		FName GetUIName();

	//UFUNCTION(BlueprintCallable, Category = Item)
	//void InitializeItem();


	//  INVENTORY  ##########################################################################

	// Handle self things when picked up.
	UFUNCTION(BlueprintCallable, Category = Inventory)
		virtual void ItemPickedUp();

	// Handle self things when dropped
	UFUNCTION(BlueprintCallable, Category = Inventory)
		virtual void ItemDropped(FVector Trajectory);


protected:

	// Hold data for UI
	UPROPERTY(EditDefaultsOnly, Category = UIData)
		FItemDataUI ItemUIData;
	
};
