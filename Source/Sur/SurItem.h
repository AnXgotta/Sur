

#pragma once

#include "GameFramework/Actor.h"
#include "SurItem.generated.h"


USTRUCT()
struct FItemUIData{

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Name)
	FName UIName;

	UPROPERTY(EditDefaultsOnly, Category = Stack)
		int32 MaxStackable;

	// defaults
	FItemUIData(){
		UIName = TEXT("Change Me in BP");
		MaxStackable = 1;

	}
};

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

	// getter for UI Data
	UFUNCTION(BlueprintCallable, Category = Item)
		FName GetUIName();


	//  INVENTORY  ##########################################################################

	// Handle self things when picked up.
	UFUNCTION(BlueprintCallable, Category = Inventory)
		virtual void ItemPickedUp();

	// Handle self things when dropped
	UFUNCTION(BlueprintCallable, Category = Inventory)
		virtual void ItemDropped(FVector Trajectory);


protected:

	// Hold data for UI
	UPROPERTY(EditDefaultsOnly, Category = Data)
		FItemUIData ItemDataUI;
	
};
