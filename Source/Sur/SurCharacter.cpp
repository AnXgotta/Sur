

#include "Sur.h"
#include "SurCharacter.h"


ASurCharacter::ASurCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	// Set size of Capsule component
	CapsuleComponent->InitCapsuleSize(42.0f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = CapsuleComponent;
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera


	Mesh1P = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	Mesh1P->AttachParent = FirstPersonCameraComponent;
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Mesh1P->RelativeLocation = FVector(0.f, 0.f, -150.f);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;


	// CHARACTER SET UP
	// Replication
	bReplicateMovement = true;
	bReplicates = true;

}


//  INVENTORY  #########################################################################

void ASurCharacter::PickUpItem(ASurItem* NewItem){

	if (!NewItem){
		PRINT_SCREEN(TEXT("NOTICE:  SSCharacter [PickUpItem] NewItem NULL"));
		return;
	}

	// check if inventory is full
	if (IsInventoryFull()){
		// see if we can autostack item ??
		PRINT_SCREEN(TEXT("NOTICE:  SSCharacter [PickUpItem] Inventory Full"));
		return;
	}

	USurInventorySlot* InventorySlot = NULL;

	// see if we can autostack item

	// add to next free slot
	InventorySlot = GetFirstEmptyInventorySlot();

	if (InventorySlot){
		InventorySlot->AddItemToSlot(NewItem, 1);
		NewItem->ItemPickedUp();
	}
	else{
		PRINT_SCREEN(TEXT("NOTICE:  SSCharacter [PickUpItem] InventorySlot NULL ?!"));
		// shouldent happen as empty slot should exist due to previous empty check
	}

}


void ASurCharacter::RemoveItemFromInventory(USurInventorySlot* RemoveItemSlot){
	if (RemoveItemSlot){
		RemoveItemSlot->RemoveItemFromSlot(1);
	}
}


void ASurCharacter::DropItem(USurInventorySlot* DropItemSlot){
	if (DropItemSlot){
		FVector CameraLocation = FirstPersonCameraComponent.Get()->GetComponentLocation();
		FVector CameraForward = FirstPersonCameraComponent.Get()->GetForwardVector();

		UWorld* const World = GetWorld();
		if (World){
			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = this;
			ASurItem* DroppedItem = World->SpawnActor<ASurItem>(DropItemSlot->ItemClass, CameraLocation + (CameraForward * 100.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
			if (DroppedItem){
				DroppedItem->ItemDropped(CameraForward);
			}
		}

	}
}


bool ASurCharacter::IsInventoryFull(){
	for (int i = 0; i < Inventory.Num(); i++){
		if (!Inventory[i]->IsSlotEmpty()){
			return false;
		}
	}
	return true;
}


USurInventorySlot* ASurCharacter::GetFirstEmptyInventorySlot(){
	for (int i = 0; i < Inventory.Num(); i++){
		if (!Inventory[i]->IsSlotEmpty()){
			return Inventory[i];
		}
	}
	return NULL;
}


//  INPUT  ################################################################################

void ASurCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent){
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	InputComponent->BindAxis("MoveForward", this, &ASurCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASurCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ASurCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ASurCharacter::LookUpAtRate);
}


//  MOVEMENT  ###########################################################################

void ASurCharacter::MoveForward(float Value){
	if (Value != 0.0f){
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASurCharacter::MoveRight(float Value){
	if (Value != 0.0f){
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASurCharacter::TurnAtRate(float Rate){
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASurCharacter::LookUpAtRate(float Rate){
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
