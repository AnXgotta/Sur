

#pragma once


#include "GameFramework/Character.h"
#include "SurItem.h"
#include "SurInventory.h"
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

	virtual void BeginPlay() OVERRIDE;
	virtual void PostInitializeComponents() OVERRIDE;
	virtual void Tick(float DeltaSeconds) OVERRIDE;


	//  SOCKET HELPOERS  #########################################################################

	const FName RIGHT_HAND_SOCKET = TEXT("RIGHT_HAND_SOCKET");



	// PLAYER STATUS ##############################################################################

	UPROPERTY()
		FSSPlayerHealthStatus PlayerStatus;

	// keep tabs on currently selected item (IN BLUEPRINT)
	UPROPERTY(BlueprintReadWrite, Category = Inventory)
		ASurItem* CurrentRightHandItem;

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




	// LINE TRACE FOR INTERACTION  #################################################################

	// [client] trace every frame for item interaction 
	UFUNCTION()
		void LineTraceForInteraction();

	// [client - server] client-> check if object is item and call .  server->  do checks, assign "CurrentlyTracedItem" for Replication
	UFUNCTION()
		void HandleLineTraceForInteractionHit(FHitResult& Hit);

	// [server]  line trace for object, call HandleLineTraceForInteractionHit
	UFUNCTION(reliable, server, WithValidation)
		void ServerHandleLineTraceForInteractionHit();

	//  INVENTORY  ##################################################################################

	UPROPERTY(Transient, Replicated)
		USurInventory* Inventory;

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

	// drop item from inventory
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void DropItem(int32 Index);

	UFUNCTION(server, reliable, WithValidation)
		void ServerDropItem(int32 Index);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void EquipItem(USurInventorySlot* EquipItemSlot);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerEquipItem(USurInventorySlot* EquipItemSlot);

	// TESTING  ##############################################################################
	// drop item from inventory
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void TestingDropFirstItem();

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void TestingEquipItem();

	// END TESTING  ##########################################################################

	
	//  INTERACTION  #########################################################################

	// keep tabs on currently equipped item
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentlyEquippedItem)
		ASurItem* CurrentlyEquippedItem;

	UFUNCTION()
		void OnRep_CurrentlyEquippedItem(ASurItem* NewEquippedItem);

	UFUNCTION()
		void UseItem();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerUseItem();


	//  INPUT  ####################################################################################

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
		void TestEquip();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) OVERRIDE;
	// End of APawn interface
	
};
