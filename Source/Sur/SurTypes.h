

#include "SurItem.h"
#include "SurTypes.generated.h"
#pragma once


USTRUCT()
struct FSSPlayerHealthStatus{
	GENERATED_USTRUCT_BODY()

	// player health
	UPROPERTY(EditDefaultsOnly, Category = AAConsumableData)
	float Health;

	// player stamina
	UPROPERTY(EditDefaultsOnly, Category = AAConsumableData)
		float Stamina;

	// player hunger
	UPROPERTY(EditDefaultsOnly, Category = AAConsumableData)
		float Hunger;

	// player thirst
	UPROPERTY(EditDefaultsOnly, Category = AAConsumableData)
		float Thirst;

	// Default Values
	FSSPlayerHealthStatus(){
		Health = 100.0f;
		Stamina = 100.0f;
		Hunger = 100.0f;
		Thirst = 100.0f;
	}
};



USTRUCT()
struct FConsumableItemData{

	GENERATED_USTRUCT_BODY()

	// player health
	UPROPERTY(EditDefaultsOnly, Category = AAConsumableData)
	float Health;

	// player stamina
	UPROPERTY(EditDefaultsOnly, Category = AAConsumableData)
		float Stamina;

	// player hunger
	UPROPERTY(EditDefaultsOnly, Category = AAConsumableData)
		float Hunger;

	// player thirst
	UPROPERTY(EditDefaultsOnly, Category = AAConsumableData)
		float Thirst;

	// time before hunger value begins decrease again
	UPROPERTY(EditDefaultsOnly, Category = AAConsumableData)
		float HungerDelay;

	UPROPERTY(EditDefaultsOnly, Category = AAConsumableData)
		float ThirstDelay;

	// Default Values
	FConsumableItemData(){
		Health = 0.0f;
		Stamina = 0.0f;
		Hunger = 0.0f;
		Thirst = 0.0f;
		HungerDelay = 0.0f;
		ThirstDelay = 0.0f;
	}



};

USTRUCT()
struct FSurButtonStruct {
	GENERATED_USTRUCT_BODY()

	//Vars
	int32 type;
	FString	toolTip;
	float minX;
	float maxX;
	float minY;
	float maxY;
	int32 index;

	//~

	//default properties

	FSurButtonStruct()	{
		type = -1;
		toolTip = "";
		minX = 0;
		maxX = 0;
		minY = 0;
		maxY = 0;
		index = -1;
	}
};

USTRUCT()
struct FCraftableItemReqs{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Name)
	FName ReqName;

	UPROPERTY(EditDefaultsOnly, Category = Amount)
		int32 Amount;

	FCraftableItemReqs(){
		//ReqName = TEXT("");
		//int32 Amount = 0;
	}
};

USTRUCT()
struct FCraftableItem {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Item)
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, Category = Item)
	TSubclassOf<class ASurItem> ItemInst;

	UPROPERTY(EditDefaultsOnly, Category = BuildTime)
		float BuildTime;

	UPROPERTY(EditDefaultsOnly, Category = NameArray)
		TArray<FCraftableItemReqs> Ingredients;

	UPROPERTY()
		int32 Index;

	UPROPERTY()
		int32 NumberCraftable;

	FCraftableItem(){
		//ItemBlueprint = NULL;
		//Ingredients = NULL;
		NumberCraftable = 0;
		Index = 0;
	}
};