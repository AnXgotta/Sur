

#pragma once


#include "GameFramework/Character.h"

#include "SurInventorySlot.h"
#include "SurItem.h"

#include "SurCharacter.generated.h"

USTRUCT()
struct FSSPlayerHealthStatus{
	GENERATED_USTRUCT_BODY()

	// player health
	UPROPERTY()
	float Health;

	// player stamina
	UPROPERTY()
		float Stamina;

	// player hunger
	UPROPERTY()
		float Hunger;

	// player thirst
	UPROPERTY()
		float Thirst;

	// Default Values
	FSSPlayerHealthStatus(){
		Health = 100.0f;
		Stamina = 100.0f;
		Hunger = 100.0f;
		Thirst = 100.0f;
	}
};

UCLASS()
class ASurCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	TSubobjectPtr<class USkeletalMeshComponent> Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		TSubobjectPtr<class UCameraComponent> FirstPersonCameraComponent;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	// PLAYER STATUS ##############################################################################

	UPROPERTY()
		FSSPlayerHealthStatus PlayerStatus;


	//  HEALTH  #####################################################################################

	// add to health


	// remove from health



	//  STAMINA  ####################################################################################



	//  HUNGER  #####################################################################################

	// add to hunger


	// remove from hunger



	// THIRST  ######################################################################################

	// add to thirst


	// remove from thirst



	//  INVENTORY  ##################################################################################

	// keep tabs on currently selected item (IN BLUEPRINT)
	UPROPERTY(BlueprintReadWrite, Category = Inventory)
		ASurItem* CurrentlyTracedItem;
	
	// player inventory
	UPROPERTY()
		TArray<USurInventorySlot* > Inventory;

	// max inventory size
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		int32 MaxInventorySize;

	// add item to inventory (IN BLUEPRINT)
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void PickUpItem(ASurItem* NewItem);

	// remove item from inventory (IN BLUEPRINT)
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void RemoveItemFromInventory(USurInventorySlot* RemoveItemSlot);

	// drop item from inventory
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void DropItem(USurInventorySlot* DropItem);

	// check if inventory full
	UFUNCTION()
		bool IsInventoryFull();

	// get first empty inventory slot
	UFUNCTION()
		USurInventorySlot* GetFirstEmptyInventorySlot();
	

	//  MOVEMENT  ####################################################################################

	UFUNCTION()
		void MoveForward(float Val);

	UFUNCTION()
		void MoveRight(float Val);

	UFUNCTION()
		void TurnAtRate(float Rate);

	UFUNCTION()
		void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) OVERRIDE;
	// End of APawn interface
	
};
