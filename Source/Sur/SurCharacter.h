

#pragma once


#include "SurTypes.h"
#include "SurItem.h"
#include "SurConsumableItem.h"
#include "SurBuildableItem.h"
#include "SurInventory.h"
#include "SurInventorySlot.h"
#include "GameFramework/Character.h"
#include "SurCharacter.generated.h"



UCLASS()
class ASurCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	/** First person camera */
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
//		TSubobjectPtr<class UCameraComponent> FirstPersonCameraComponent;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	virtual void BeginPlay() OVERRIDE;
	virtual void PostInitializeComponents() OVERRIDE;
	virtual void Tick(float DeltaSeconds) OVERRIDE;


	//  SOCKET HELPOERS  #########################################################################

	const FName RIGHT_HAND_SOCKET = TEXT("RIGHT_HAND_SOCKET");



	// PLAYER STATUS ##############################################################################

	UPROPERTY(Transient, Replicated)
		FSSPlayerHealthStatus PlayerStatus;

	// keep tabs on currently selected item (IN BLUEPRINT)
	UPROPERTY(BlueprintReadWrite, Category = Inventory)
		ASurItem* CurrentRightHandItem;

	//  HEALTH  #####################################################################################

	// add to health

	// remove from health



	//  STAMINA  ####################################################################################



	//  HUNGER  #####################################################################################

	UPROPERTY()
		bool bDecreaseHunger;

	UPROPERTY()
		float HungerDecreasePerMinute;

	UFUNCTION()
		void EnableHungerDecrease();

	UFUNCTION()
		void DisableHungerDecreaseForTime(float Minutes);

	UFUNCTION()
		void DecreaseHungerValue(float DeltaSeconds);


	// THIRST  ######################################################################################

	UPROPERTY()
		bool bDecreaseThirst;

	UPROPERTY()
		float ThirstDecreasePerMinute;

	UFUNCTION()
		void EnableThirstDecrease();

	UFUNCTION()
		void DisableThirstDecreaseForTime(float Minutes);

	UFUNCTION()
		void DecreaseThirstValue(float DeltaSeconds);



	//  ITEM SPECIFIC INTERATION  ##################################################################

	
	UFUNCTION()
		void HandleConsumableItemData(FConsumableItemData CItemData);


	// LINE TRACE FOR INTERACTION  #################################################################

	// [client] trace every frame for item interaction 
	UFUNCTION()
		void LineTraceForInteraction();

	// [client - server] client-> check if object is item and call .  server->  do checks, assign "CurrentlyTracedItem" for Replication
	UFUNCTION()
		void HandleLineTraceForInteractionHit(FHitResult& Hit);

	// [server]  line trace for object, call HandleLineTraceForInteractionHit
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerHandleLineTraceForInteractionHit();

	//  INVENTORY  ##################################################################################

	UPROPERTY(Transient, Replicated)
		USurInventory* Inventory;

	UPROPERTY(Transient, Replicated)
		USurInventory* ActionBar;

	UPROPERTY(Transient, Replicated)
		int32 ActionBarIndex;

	UFUNCTION()
		void AdjustActionBarIndex(int32 Adjust);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAdjustActionBarIndex(int32 Adjust);

	// keep tabs on currently selected item
	UPROPERTY(Transient, Replicated)
		ASurItem* CurrentlyTracedItem;

	UFUNCTION(BlueprintCallable, Category = Inventory)
		TArray<USurInventorySlot*> GetInventory();

	// add item to inventory (IN BLUEPRINT)
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void PickUpItem();

	UFUNCTION(server, reliable, WithValidation)
		void ServerPickUpItem();

	UFUNCTION()
		bool ServerAddCraftedItemToInventory(FCraftableItem CraftedItem);		
		
		// drop item from inventory
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void DropEquippedItem();

	UFUNCTION(server, reliable, WithValidation)
		void ServerDropEquippedItem();

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void EquipItem(USurInventorySlot* EquipItemSlot);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerEquipItem(USurInventorySlot* EquipItemSlot);

	UFUNCTION()
		bool CanOpenInventoryUI();

	//  BUILDING  ###########################################################################

	UPROPERTY(Transient, Replicated)
		bool bIsBuilding;

	UPROPERTY()
		float BuildingDistanceModifier;

	UPROPERTY()
		float BuildingRotationModifier;

	UFUNCTION()
		void BuildingTickHandle(float DeltaSeconds);

	UFUNCTION()
		void BuildProcessBegin();

	UFUNCTION()
		void BuildProcessEnd(bool Cancelled);

	
	//  INTERACTION  #########################################################################

	// keep tabs on currently equipped item
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentlyEquippedItem)
		ASurItem* CurrentlyEquippedItem;

	UPROPERTY(Transient, Replicated)
		USurInventorySlot* CurrentlyEquippedInventorySlot;

	UFUNCTION()
		void OnRep_CurrentlyEquippedItem(ASurItem* NewEquippedItem);

	UFUNCTION()
		void UseItem(bool bVoid);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerUseItem(bool bVoid);

	UFUNCTION()
		void HandleInventoryUITransaction(USurInventorySlot* FromSlot, USurInventorySlot* ToSlot);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerHandleInventoryUITransaction(USurInventorySlot* FromSlot, USurInventorySlot* ToSlot);


	//  INPUT  ####################################################################################

	UPROPERTY()
		bool bEnableInput;

	UFUNCTION()
		void MoveForward(float Val);

	UFUNCTION()
		void MoveRight(float Val);

	UFUNCTION()
		void TurnAtRate(float Rate);

	UFUNCTION()
		void LookUpAtRate(float Rate);

	UFUNCTION()
		void Drop();

	UFUNCTION()
		void Interact();

	UFUNCTION()
		void ActionQPress();

	UFUNCTION()
		void ActionQRelease();

	UPROPERTY()
		bool bIsQHeld;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerIsQHeld(bool IsHeld);

	UFUNCTION()
		void ActionEPress();

	UFUNCTION()
		void ActionERelease();

	UPROPERTY()
		bool bIsEHeld;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerIsEHeld(bool IsHeld);

	UFUNCTION()
		void LMB();

	UFUNCTION()
		void RMB();

	UFUNCTION()
		void MWU();

	UFUNCTION()
		void MWD();

	UFUNCTION()
		void Button1();

	UFUNCTION()
		void Button2();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) OVERRIDE;
	// End of APawn interface
	
};
