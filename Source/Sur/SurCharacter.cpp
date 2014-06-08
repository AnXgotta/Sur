

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


void ASurCharacter::BeginPlay(){
	Inventory = NewObject<USurInventory>();
	Inventory->Initialize();
	Super::BeginPlay();	
}


void ASurCharacter::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);
	
}


//  INVENTORY  #########################################################################



void ASurCharacter::PickUpItem(){
	if (!CurrentlyTracedItem){
		PRINT_SCREEN("SurCharacter:  [PickUpItem]  No Item Traced!");
		return;
	}
	if (!Inventory->AddItem(CurrentlyTracedItem)){
		// most likely inventory is full... or an error of some kind
		PRINT_SCREEN("SurCharacter:  [PickUpItem]  Inventory Full!");
	}
}


void ASurCharacter::TestingDropFirstItem(){
	for (int i = 0; i < Inventory->MaxSize; i++){
		if (!Inventory->Inventory[i]->IsSlotEmpty()){
			DropItem(Inventory->Inventory[i]);
			return;
		}
	}
}

TArray<USurInventorySlot*> ASurCharacter::GetInventory(){
	return Inventory->Inventory;
}

void ASurCharacter::DropItem(USurInventorySlot* OldItemSlot){
	if (OldItemSlot){
		FVector CameraLocation = FirstPersonCameraComponent.Get()->GetComponentLocation();
		FVector CameraForward = FirstPersonCameraComponent.Get()->GetForwardVector();

		UWorld* const World = GetWorld();
		if (World){
			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = this;
			ASurItem* DroppedItem = World->SpawnActor<ASurItem>(OldItemSlot->ItemBlueprint, CameraLocation + (CameraForward * 100.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
			if (DroppedItem){
				Inventory->RemoveItem(OldItemSlot);
				DroppedItem->ItemDropped(CameraForward);
			}
		}

	}
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
