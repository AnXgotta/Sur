

#include "Sur.h"
#include "SurPlayerController.h"
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
	Mesh1P->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh1P->PrimaryComponentTick.TickGroup = TG_PrePhysics;
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
	Super::BeginPlay();	
}

void ASurCharacter::PostInitializeComponents(){
	Super::PostInitializeComponents();

	if (Role == ROLE_Authority){
		Inventory = NewObject<USurInventory>();
		Inventory->Initialize();
	}

}

void ASurCharacter::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);
	
	LineTraceForInteraction();

	

}




//  LINE TRACE FOR INTERACTION  #######################################################

void ASurCharacter::LineTraceForInteraction(){

	if (!Controller) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	Controller->GetPlayerViewPoint(CameraLocation, CameraRotation);

	const FVector StartPoint = CameraLocation;
	const FVector EndPoint = CameraLocation + (CameraRotation.Vector() * 200.0f);

	FHitResult HitOut;
	HitOut = FHitResult(ForceInit);
	ECollisionChannel traceCollisionChannel = ECC_GameTraceChannel1;
	FCollisionQueryParams traceParams(FName(TEXT("Interaction Trace")), true, NULL);
	traceParams.bTraceComplex = true;
	DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, -1.0f, NULL, 2.0f);

	//-MOVE THIS TO TOP OF FUNCTION WHEN DONE DEBUGGING-----
	if (Role == ROLE_Authority) return;
	//------------------------------------------------------

	if (GetWorld()->LineTraceSingle(HitOut, StartPoint, EndPoint, traceCollisionChannel, traceParams)){
		HandleLineTraceForInteractionHit(HitOut);
	}else{
		if (CurrentlyTracedItem){
			ServerHandleLineTraceForInteractionHit();			
		}
	}
}


void ASurCharacter::HandleLineTraceForInteractionHit(FHitResult& Hit){
	ASurItem* ItemHit = Cast<ASurItem>(Hit.GetActor());
	if (!ItemHit) return;

	if (CurrentlyTracedItem == ItemHit){
		return;
	}

	if (Role == ROLE_Authority){
		CurrentlyTracedItem = ItemHit;
	}else{
		ServerHandleLineTraceForInteractionHit();
	}
		
}

bool ASurCharacter::ServerHandleLineTraceForInteractionHit_Validate(){
	return true;
}

void ASurCharacter::ServerHandleLineTraceForInteractionHit_Implementation(){
	if (!Controller) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	Controller->GetPlayerViewPoint(CameraLocation, CameraRotation);

	const FVector StartPoint = CameraLocation;
	const FVector EndPoint = CameraLocation + (CameraRotation.Vector() * 200.0f);

	FHitResult HitOut;
	ECollisionChannel traceCollisionChannel = ECC_GameTraceChannel1;
	FCollisionQueryParams traceParams(FName(TEXT("Interaction Trace")), true, NULL);
	traceParams.bTraceComplex = true;

	if (GetWorld()->LineTraceSingle(HitOut, StartPoint, EndPoint, traceCollisionChannel, traceParams)){
		HandleLineTraceForInteractionHit(HitOut);
	}else{
		if (CurrentlyTracedItem){
			CurrentlyTracedItem = NULL;
		}
	}
}





//  INVENTORY  #########################################################################

TArray<USurInventorySlot*> ASurCharacter::GetInventory(){
	return Inventory->Inventory;
}

void ASurCharacter::PickUpItem(){

	PRINT_SCREEN("SurCharacter:  [PickUpItem] Called");
	if (Role == ROLE_Authority){
		if (!CurrentlyTracedItem){
			PRINT_SCREEN("SurCharacter:  [PickUpItem]  No Item Traced!");
			return;
		}

		if (!Inventory->AddItem(CurrentlyTracedItem)){
			// most likely inventory is full... or an error of some kind
			PRINT_SCREEN("SurCharacter:  [PickUpItem]  Inventory Full!");
		}
	}
	else{
		ServerPickUpItem();
	}
}

bool ASurCharacter::ServerPickUpItem_Validate(){
	return true;
}

void ASurCharacter::ServerPickUpItem_Implementation(){
	PickUpItem();
}

void ASurCharacter::DropItem(int32 Index){

	USurInventorySlot* OldItemSlot = Inventory->Inventory[Index];
	if (!OldItemSlot) return;

	if (Role != ROLE_Authority){
		ServerDropItem(Index);
		return;
	}

	if (!Controller) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	Controller->GetPlayerViewPoint(CameraLocation, CameraRotation);
	const FVector SpawnPoint = CameraLocation + (CameraRotation.Vector() * 100.0f);

	UWorld* const World = GetWorld();
	if (World){
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = this;
		ASurItem* DroppedItem = World->SpawnActor<ASurItem>(OldItemSlot->ItemBlueprint, SpawnPoint, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
		if (DroppedItem){
			Inventory->RemoveItem(Index);
			DroppedItem->ItemDropped(CameraRotation.Vector());
		}
	}

}

bool ASurCharacter::ServerDropItem_Validate(int32 Index){
	return true;
}

void ASurCharacter::ServerDropItem_Implementation(int32 Index){
	DropItem(Index);
}



//  TESTING ################################################################
void ASurCharacter::TestingDropFirstItem(){
	for (int i = 0; i < Inventory->MaxSize; i++){
		if (!Inventory->Inventory[i]->IsSlotEmpty()){
			DropItem(i);
			return;
		}
	}
}

void ASurCharacter::TestingEquipItem(){
	PRINT_SCREEN("ASurCharacter [TestintEquipItem] Called");
	for (int i = 0; i < Inventory->MaxSize; i++){
		if (!Inventory->Inventory[i]->IsSlotEmpty()){
			EquipItem(Inventory->Inventory[i]);
			return;
		}
	}
}

//  END TESTING  ###########################################################

//  INTERACTION  ###########################################################

void ASurCharacter::UseItem(){
	if (!CurrentlyEquippedItem){
		PRINT_SCREEN("ASurCharacter [UseItem] CurrentlyEquippedItem is NULL");
		return;
	}

	if (Role < ROLE_Authority){
		ServerUseItem();
	}

	CurrentlyEquippedItem->OnUseItem();

}

bool ASurCharacter::ServerUseItem_Validate(){
	return true;
}

void ASurCharacter::ServerUseItem_Implementation(){

}

void ASurCharacter::OnRep_CurrentlyEquippedItem(ASurItem* LastEquippedItem){
	// unequip current item
	if (LastEquippedItem){
		LastEquippedItem->OnItemUnEquipped();
	}

	if (!CurrentlyEquippedItem) return;

	CurrentlyEquippedItem->OnItemEquipped();
	CurrentlyEquippedItem->Mesh->AttachTo(Mesh1P, RIGHT_HAND_SOCKET, EAttachLocation::SnapToTarget);
	PRINT_SCREEN("ASurCharacter [OnRep_CurrentlyEquippedItem] Client Replicated");
}


void ASurCharacter::EquipItem(USurInventorySlot* EquipItemSlot){
	if (!EquipItemSlot) return;
	if (!Mesh1P) return;

	if (Role < ROLE_Authority){
		PRINT_SCREEN("ASurCharacter [EquipItem] Client");
		ServerEquipItem(EquipItemSlot);
	}
}

bool ASurCharacter::ServerEquipItem_Validate(USurInventorySlot* EquipItemSlot){
	return true;
}

void ASurCharacter::ServerEquipItem_Implementation(USurInventorySlot* EquipItemSlot){
	// unequip current item
	if (CurrentlyEquippedItem){
		CurrentlyEquippedItem->OnItemUnEquipped();
	}

	if (!EquipItemSlot) return;

	UWorld* const World = GetWorld();
	if (World){
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = this;
		ASurItem* NewSpawnedItem = World->SpawnActor<ASurItem>(EquipItemSlot->ItemBlueprint, Mesh1P->GetSocketLocation(RIGHT_HAND_SOCKET), Mesh1P->GetSocketRotation(RIGHT_HAND_SOCKET));
		if (NewSpawnedItem){
			CurrentlyEquippedItem = NewSpawnedItem;
			CurrentlyEquippedItem->OnItemEquipped();
			CurrentlyEquippedItem->Mesh->AttachTo(Mesh1P, RIGHT_HAND_SOCKET, EAttachLocation::SnapToTarget);
			PRINT_SCREEN("SurCharacter [EquipItem]  Fully Equipped");
		}
	}
	PRINT_SCREEN("ASurCharacter [ServerEquipItem_Implementation] Server");
}


//  INPUT  ################################################################################

void ASurCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent){
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Interact", IE_Pressed, this, &ASurCharacter::Interact);
	InputComponent->BindAction("Drop", IE_Pressed, this, &ASurCharacter::Drop);
	InputComponent->BindAction("TestEquip", IE_Pressed, this, &ASurCharacter::TestEquip);
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


//  INPUT  ###########################################################################

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

void ASurCharacter::Drop(){
	TestingDropFirstItem();
}

void ASurCharacter::Interact(){
	PickUpItem();
}

void ASurCharacter::TestEquip(){
	PRINT_SCREEN("ASurCharacter [TestEquip] Called");
	TestingEquipItem();
}


//  REPLICATION SETTINGS  ####################################################################

void ASurCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//enum ELifetimeCondition{COND_None = 0,COND_InitialOnly = 1,COND_OwnerOnly = 2,COND_SkipOwner = 3,COND_SimulatedOnly = 4,
	//COND_AutonomousOnly = 5,COND_SimulatedOrPhysics = 6,COND_InitialOrOwner = 7,COND_Custom = 8,COND_Max = 9,}

	/*	COND_InitialOnly - This property will only attempt to send on the initial bunch
	COND_OwnerOnly - This property will only send to the actor's owner
	COND_SkipOwner - This property send to every connection EXCEPT the owner
	COND_SimulatedOnly - This property will only send to simulated actors
	COND_AutonomousOnly - This property will only send to autonomous actors
	COND_SimulatedOrPhysics - This property will send to simulated OR bRepPhysics actors
	COND_InitialOrOwner - This property will send on the initial packet, or to the actors owner
	COND_Custom - This property has no particular condition, but wants the ability to toggle on / off via SetCustomIsActiveOverride
	*/

	// only to local owner: weapon change requests are locally instigated, other clients don't need it
	DOREPLIFETIME_CONDITION(ASurCharacter, Inventory, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ASurCharacter, CurrentlyTracedItem, COND_OwnerOnly);

	DOREPLIFETIME(ASurCharacter, CurrentlyEquippedItem);
}


