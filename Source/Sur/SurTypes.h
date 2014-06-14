
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
