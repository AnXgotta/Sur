

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

//	Mesh->AttachParent = RootComponent;
	Mesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;

	/*// Create a CameraComponent	
	FirstPersonCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = Mesh;
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	*/
	// CHARACTER SET UP

	// hunger
	HungerDecreasePerMinute = 50.0f;
	bDecreaseHunger = false;
	// thirst
	ThirstDecreasePerMinute = 50.0f;
	bDecreaseThirst = false;

	bIsBuilding = false;
	bIsQHeld = false;
	bIsEHeld = false;
	BuildingDistanceModifier = 1.0f;
	BuildingRotationModifier = 0.0f;


	bEnableInput = true;
	ActionBarIndex = 0;

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
		ActionBar = NewObject<USurInventory>();

		Inventory->Initialize(32);
		ActionBar->Initialize(8);
		bDecreaseHunger = true;
		bDecreaseThirst = true;
	}

}

void ASurCharacter::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);
	LineTraceForInteraction();

	BuildingTickHandle(DeltaSeconds);

	DecreaseHungerValue(DeltaSeconds);
	DecreaseThirstValue(DeltaSeconds);
}



//  PLAYER STATUS  ###########################################################################

// Hunger

void ASurCharacter::EnableHungerDecrease(){
	bDecreaseHunger = true;
}

void ASurCharacter::DisableHungerDecreaseForTime(float Minutes){
	bDecreaseHunger = false;
	GetWorldTimerManager().SetTimer(this, &ASurCharacter::EnableHungerDecrease, Minutes * 60.0f, false);
}

void ASurCharacter::DecreaseHungerValue(float DeltaSeconds){
	if (!bDecreaseHunger) return;
	PlayerStatus.Hunger = FMath::Clamp(PlayerStatus.Hunger - (DeltaSeconds * (HungerDecreasePerMinute/60.0f)), 0.0f, 100.0f);
}

// Thirst

void ASurCharacter::EnableThirstDecrease(){
	bDecreaseThirst = true;
}

void ASurCharacter::DisableThirstDecreaseForTime(float Minutes){
	bDecreaseThirst = false;
	GetWorldTimerManager().SetTimer(this, &ASurCharacter::EnableThirstDecrease, Minutes * 60.0f, false);
}

void ASurCharacter::DecreaseThirstValue(float DeltaSeconds){
	if (!bDecreaseThirst) return;
	PlayerStatus.Thirst = FMath::Clamp(PlayerStatus.Thirst - (DeltaSeconds * (ThirstDecreasePerMinute / 60.0f)), 0.0f, 100.0f);
}




//  ITEM SPECIFIC INTERACTION  ##################################################################



void ASurCharacter::HandleConsumableItemData(FConsumableItemData CItemData){
	PlayerStatus.Health = FMath::Clamp(PlayerStatus.Health + CItemData.Health, 0.0f, 100.0f);
	PlayerStatus.Stamina = FMath::Clamp(PlayerStatus.Stamina + CItemData.Stamina, 0.0f, 100.0f);
	PlayerStatus.Hunger = FMath::Clamp(PlayerStatus.Hunger + CItemData.Hunger, 0.0f, 100.0f);
	PlayerStatus.Thirst = FMath::Clamp(PlayerStatus.Thirst + CItemData.Thirst, 0.0f, 100.0f);

	if (CItemData.HungerDelay > 0.0f){
		DisableHungerDecreaseForTime(CItemData.HungerDelay);
	}

	if (CItemData.ThirstDelay > 0.0f){
		DisableThirstDecreaseForTime(CItemData.ThirstDelay);
	}


	CurrentlyEquippedInventorySlot->RemoveItemFromSlot(1);
	if (CurrentlyEquippedInventorySlot->IsSlotEmpty()){
		EquipItem(CurrentlyEquippedInventorySlot);
	}
}


//  LINE TRACE FOR INTERACTION  #######################################################


void ASurCharacter::LineTraceForInteraction(){
	if (Role == ROLE_Authority) return;
	if (!Controller) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	Controller->GetPlayerViewPoint(CameraLocation, CameraRotation);

	const FVector StartPoint = CameraLocation;
	const FVector EndPoint = CameraLocation + (CameraRotation.Vector() * 250.0f);


	FHitResult HitOut;
	HitOut = FHitResult(ForceInit);
	ECollisionChannel traceCollisionChannel = TRACE_CHANNEL_ITEM;
	FCollisionQueryParams traceParams(FName(TEXT("Interaction Trace")), true, NULL);
	traceParams.bTraceComplex = true;

	

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

		// try stack in ActionBar, stack in inventory, open slot in actionbar, finally open slot in inventory
		if (ActionBar->AddItemToInventoryFromItemStack(CurrentlyTracedItem) > 0){
			if (Inventory->AddItemToInventoryFromItemStack(CurrentlyTracedItem) > 0){
				if (ActionBar->AddItemToInventoryFromItemOpen(CurrentlyTracedItem) > 0){
					if (Inventory->AddItemToInventoryFromItemOpen(CurrentlyTracedItem) > 0){
						// most likely inventory is full... or an error of some kind
						PRINT_SCREEN("SurCharacter:  [PickUpItem]  Inventory Full!");
						return;
					}					
				}
			}
		}
		if (!CurrentlyEquippedItem){
			PRINT_SCREEN("Equipped pickup");
			EquipItem(ActionBar->Inventory[ActionBarIndex]);
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

bool ASurCharacter::ServerAddCraftedItemToInventory(FCraftableItem CraftedItem){

	return true;
}

void ASurCharacter::DropEquippedItem(){

	if (!CurrentlyEquippedItem) return;

	if (Role < ROLE_Authority){
		ServerDropEquippedItem();
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
		ASurItem* DroppedItem = World->SpawnActor<ASurItem>(CurrentlyEquippedItem->SurItemBlueprint, SpawnPoint, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
		if (DroppedItem){
			CurrentlyEquippedInventorySlot->RemoveItemFromSlot(1);
			DroppedItem->ItemDropped(CameraRotation.Vector());
			if (CurrentlyEquippedInventorySlot->IsSlotEmpty()){
				CurrentlyEquippedInventorySlot = NULL;
				CurrentlyEquippedItem->OnItemUnEquipped();
				CurrentlyEquippedItem = NULL;
			}
		}
	}

}

bool ASurCharacter::ServerDropEquippedItem_Validate(){
	return true;
}

void ASurCharacter::ServerDropEquippedItem_Implementation(){
	DropEquippedItem();
}

bool ASurCharacter::CanOpenInventoryUI(){
	return !bIsBuilding;
}

void ASurCharacter::AdjustActionBarIndex(int32 Adjust){
	if (Role < ROLE_Authority){
		ServerAdjustActionBarIndex(Adjust);
		return;
	}

	ActionBarIndex = Adjust;

	EquipItem(ActionBar->Inventory[ActionBarIndex]);

}

bool ASurCharacter::ServerAdjustActionBarIndex_Validate(int32 Adjust){
	return true;
}

void ASurCharacter::ServerAdjustActionBarIndex_Implementation(int32 Adjust){
	AdjustActionBarIndex(Adjust);
}


//  BUILDING  ##############################################################


void ASurCharacter::BuildingTickHandle(float DeltaSeconds){
	if (!bIsBuilding) return;

	FVector CurrentBuildPosition = CapsuleComponent->GetComponentLocation() + (CapsuleComponent->GetForwardVector() * (300.0f * BuildingDistanceModifier));
	FRotator CurrentBuildRotation = FRotator::ZeroRotator;

	FVector CameraLocation;
	FRotator CameraRotation;
	Controller->GetPlayerViewPoint(CameraLocation, CameraRotation);

	const FVector StartPoint = CameraLocation;
	const FVector EndPoint = CameraLocation + (CameraRotation.Vector() * 800.0f);

	FHitResult HitOut;
	HitOut = FHitResult(ForceInit);
	ECollisionChannel traceCollisionChannel = ECC_WorldStatic;
	FCollisionQueryParams traceParams(FName(TEXT("Build Trace")), true, NULL);
	traceParams.bTraceComplex = true;
	traceParams.AddIgnoredActor(CurrentlyEquippedItem);
	traceParams.AddIgnoredActor(this);

	DrawDebugLine(GetWorld(), CurrentlyEquippedItem->GetActorLocation(), CurrentlyEquippedItem->GetActorLocation() + CurrentlyEquippedItem->GetActorUpVector() * 300.0f, FColor::Red);

	if (GetWorld()->LineTraceSingle(HitOut, StartPoint, EndPoint, traceCollisionChannel, traceParams)){
		CurrentBuildPosition = HitOut.Location;
		CurrentBuildRotation = HitOut.Normal.Rotation() + FRotator(-90.0f, 0.0f, 0.0f);	
	}	

	CurrentlyEquippedItem->SetActorLocation(CurrentBuildPosition);
	CurrentlyEquippedItem->SetActorRotation(CurrentBuildRotation);

	if (bIsQHeld){
		BuildingRotationModifier = BuildingRotationModifier + 30.0f * DeltaSeconds;
	}

	if (bIsEHeld){
		BuildingRotationModifier = BuildingRotationModifier - 30.0f * DeltaSeconds;
	}
	
	CurrentlyEquippedItem->AddActorLocalRotation(FRotator(0.0f, BuildingRotationModifier + CapsuleComponent->GetComponentRotation().Yaw, 0.0f));

}

// [server]
void ASurCharacter::BuildProcessBegin(){
	bIsBuilding = true;
	ASurBuildableItem* CurBuildable = Cast<ASurBuildableItem>(CurrentlyEquippedItem);
	if (!CurBuildable){
		// should never happen
		PRINT_SCREEN("ASurCharacter [BuildProcessBegin] ShouldNeverHappen");
		return;
	}
	CurBuildable->OnBeginBuilding();
}


void ASurCharacter::BuildProcessEnd(bool Cancelled){
	bIsBuilding = false;
	ASurBuildableItem* CurBuildable = Cast<ASurBuildableItem>(CurrentlyEquippedItem);
	if (!CurBuildable){
		// should never happen
		PRINT_SCREEN("ASurCharacter [BuildProcessEnd] ShouldNeverHappen");
		return;
	}

	

	if (Cancelled){
		CurBuildable->OnEndBuilding(Cancelled);
		CurrentlyEquippedItem->SetActorLocationAndRotation(Mesh->GetSocketLocation(RIGHT_HAND_SOCKET), Mesh->GetSocketRotation(RIGHT_HAND_SOCKET));
		PRINT_SCREEN("ASurCharacter [BuildProcessEnd] Cancelled");
	}
	else{
		if (!CurBuildable->CanBuild()){
			bIsBuilding = true;
			return;
		}

		CurBuildable->OnEndBuilding(Cancelled);

		const FVector SpawnPoint = CurrentlyEquippedItem->GetActorLocation();

		UWorld* const World = GetWorld();
		if (World){
			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = this;
			ASurItem* DroppedItem = World->SpawnActor<ASurItem>(CurrentlyEquippedItem->SurItemBlueprint, CurrentlyEquippedItem->GetActorLocation(), CurrentlyEquippedItem->GetActorRotation(), SpawnParams);
			if (DroppedItem){
				CurrentlyEquippedInventorySlot->RemoveItemFromSlot(1);
				if (CurrentlyEquippedInventorySlot->IsSlotEmpty()){
					CurrentlyEquippedInventorySlot = NULL;
					CurrentlyEquippedItem->OnItemUnEquipped();
					CurrentlyEquippedItem = NULL;
				}
			}
		}
	}
}

bool ASurCharacter::ServerBuildProcessEnd_Validate(bool Cancelled){
	return true;
}

void ASurCharacter::ServerBuildProcessEnd_Implementation(bool Cancelled){

}


//  INTERACTION  ###########################################################


void ASurCharacter::UseItem(bool bVoid){
	if (!CurrentlyEquippedItem){
		return;
	}

	if (Role < ROLE_Authority){
		ServerUseItem(bVoid);
		return;
	}

	ASurConsumableItem* CItem = NULL;

	switch (CurrentlyEquippedItem->GetItemActionType()){
	case EItemAction::Build:
		if (!bIsBuilding){
			BuildProcessBegin();
		}else{
			BuildProcessEnd(bVoid);
		}
		
		break;
	case EItemAction::Consume:
		 CItem = Cast<ASurConsumableItem>(CurrentlyEquippedItem);
		if (!CItem) return;
		HandleConsumableItemData(CItem->ConsumableData);
		CItem->OnUseItem();
		
		break;
	default:

		break;
	}


	

}

bool ASurCharacter::ServerUseItem_Validate(bool bVoid){
	switch (CurrentlyEquippedItem->GetItemActionType()){
	case EItemAction::Build:

		break;
	case EItemAction::Consume:
		
		break;
	default:

		break;
	}
	return true;
}

void ASurCharacter::ServerUseItem_Implementation(bool bVoid){
	UseItem(bVoid);
}

void ASurCharacter::OnRep_CurrentlyEquippedItem(ASurItem* LastEquippedItem){
	// unequip current item
	if (LastEquippedItem){
		LastEquippedItem->OnItemUnEquipped();
	}

	if (!CurrentlyEquippedItem) return;

	CurrentlyEquippedItem->OnItemEquipped();	

	CurrentlyEquippedItem->Mesh->AttachTo(Mesh, RIGHT_HAND_SOCKET, EAttachLocation::SnapToTarget);
	PRINT_SCREEN("ASurCharacter [OnRep_CurrentlyEquippedItem] Client Replicated");
}

void ASurCharacter::EquipItem(USurInventorySlot* EquipItemSlot){
	if (!EquipItemSlot) return;
	if (!Mesh) return;

	if (Role < ROLE_Authority){
		ServerEquipItem(EquipItemSlot);
	}
	else{
		// unequip current item
		if (CurrentlyEquippedItem){
			CurrentlyEquippedItem->OnItemUnEquipped();
			CurrentlyEquippedItem = NULL;
			CurrentlyEquippedInventorySlot = NULL;
		}

		if (!EquipItemSlot || EquipItemSlot->IsSlotEmpty()) return;

		UWorld* const World = GetWorld();
		if (World){
			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = this;
			ASurItem* NewSpawnedItem = World->SpawnActor<ASurItem>(EquipItemSlot->ItemBlueprint, Mesh->GetSocketLocation(RIGHT_HAND_SOCKET), Mesh->GetSocketRotation(RIGHT_HAND_SOCKET));
			if (NewSpawnedItem){
				CurrentlyEquippedItem = NewSpawnedItem;
				CurrentlyEquippedItem->OnItemEquipped();
				CurrentlyEquippedInventorySlot = EquipItemSlot;
				CurrentlyEquippedItem->BaseComponent->AttachTo(Mesh, RIGHT_HAND_SOCKET, EAttachLocation::SnapToTarget);
			}
		}
	}
}

bool ASurCharacter::ServerEquipItem_Validate(USurInventorySlot* EquipItemSlot){
	return true;
}

void ASurCharacter::ServerEquipItem_Implementation(USurInventorySlot* EquipItemSlot){
	EquipItem(EquipItemSlot);
}

void ASurCharacter::HandleInventoryUITransaction(USurInventorySlot* FromSlot, USurInventorySlot* ToSlot){
	if (!FromSlot || !ToSlot) return;

	if (Role < ROLE_Authority){
		ServerHandleInventoryUITransaction(FromSlot, ToSlot);
		return;
	}
	if (ToSlot->IsSlotEmpty()){
		ToSlot->SetSlotInformationFromSlot(FromSlot);
		FromSlot->ClearSlotInformation();
	}else if (FromSlot->ItemBlueprint == ToSlot->ItemBlueprint){
		if (FromSlot->NumberItemsStacked <= ToSlot->SpaceRemaining()){
			ToSlot->NumberItemsStacked += FromSlot->NumberItemsStacked;
			FromSlot->ClearSlotInformation();
		}else{
			FromSlot->NumberItemsStacked -= ToSlot->SpaceRemaining();
			ToSlot->NumberItemsStacked = ToSlot->MaxStackableInSlot;
		}
	}
	else{
		ToSlot->SwapSlotInformation(FromSlot);
	}


	if (!CurrentlyEquippedItem || CurrentlyEquippedItem->SurItemBlueprint != ActionBar->Inventory[ActionBarIndex]->ItemBlueprint){
		EquipItem(ActionBar->Inventory[ActionBarIndex]);
	}
}

bool ASurCharacter::ServerHandleInventoryUITransaction_Validate(USurInventorySlot* FromSlot, USurInventorySlot* ToSlot){
	return true;
}

void ASurCharacter::ServerHandleInventoryUITransaction_Implementation(USurInventorySlot* FromSlot, USurInventorySlot* ToSlot){
	HandleInventoryUITransaction(FromSlot, ToSlot);
}


//  INPUT  ################################################################################


void ASurCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent){
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Interact", IE_Pressed, this, &ASurCharacter::Interact);
	InputComponent->BindAction("Drop", IE_Pressed, this, &ASurCharacter::Drop);
	InputComponent->BindAction("Q", IE_Pressed, this, &ASurCharacter::ActionQPress);
	InputComponent->BindAction("Q", IE_Released, this, &ASurCharacter::ActionQRelease);
	InputComponent->BindAction("E", IE_Pressed, this, &ASurCharacter::ActionEPress);
	InputComponent->BindAction("E", IE_Released, this, &ASurCharacter::ActionERelease);
	InputComponent->BindAction("LMB", IE_Pressed, this, &ASurCharacter::LMB);
	InputComponent->BindAction("RMB", IE_Pressed, this, &ASurCharacter::RMB);
	InputComponent->BindAction("MWU", IE_Pressed, this, &ASurCharacter::MWU);
	InputComponent->BindAction("MWD", IE_Pressed, this, &ASurCharacter::MWD);

	InputComponent->BindAction("Button1", IE_Pressed, this, &ASurCharacter::Button1);
	InputComponent->BindAction("Button2", IE_Pressed, this, &ASurCharacter::Button2);

	InputComponent->BindAxis("MoveForward", this, &ASurCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASurCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}


//  INPUT  ###########################################################################


void ASurCharacter::MoveForward(float Value){
	if (!bEnableInput) return;
	if (Value != 0.0f){
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASurCharacter::MoveRight(float Value){
	if (!bEnableInput) return;
	if (Value != 0.0f){
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASurCharacter::TurnAtRate(float Rate){
	if (!bEnableInput) return;
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASurCharacter::LookUpAtRate(float Rate){
	if (!bEnableInput) return;
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASurCharacter::Drop(){
	if (!bEnableInput) return;
	DropEquippedItem();
}

void ASurCharacter::Interact(){
	if (!bEnableInput) return;
	PickUpItem();
}

void ASurCharacter::ActionQPress(){
	if (!bEnableInput) return;
	if (bIsBuilding){
		bIsQHeld = true;
		ServerIsQHeld(true);		
	}
}

void ASurCharacter::ActionQRelease(){
	if (!bEnableInput) return;
	if (bIsBuilding){
		bIsQHeld = false;
		ServerIsQHeld(false);
	}
}

bool ASurCharacter::ServerIsQHeld_Validate(bool IsHeld){
	return true;
}

void ASurCharacter::ServerIsQHeld_Implementation(bool IsHeld){
	bIsQHeld = IsHeld;
}

void ASurCharacter::ActionEPress(){
	if (!bEnableInput) return;
	if (bIsBuilding){
		bIsEHeld = true;
		ServerIsEHeld(true);
	}
}

void ASurCharacter::ActionERelease(){
	if (!bEnableInput) return;
	if (bIsBuilding){
		bIsEHeld = false;
		ServerIsEHeld(false);
	}
}

bool ASurCharacter::ServerIsEHeld_Validate(bool IsHeld){
	return true;
}

void ASurCharacter::ServerIsEHeld_Implementation(bool IsHeld){
	bIsEHeld = IsHeld;
}

void ASurCharacter::LMB(){
	if (!bEnableInput) return;
	UseItem(false);
}

void ASurCharacter::RMB(){
	if (!bEnableInput) return;
	if (bIsBuilding){
		UseItem(true);
	}
}

void ASurCharacter::MWU(){
	if (!bEnableInput) return;



	if (bIsBuilding) return;
	int32 NewABIndex = ActionBarIndex + 1;
	if (NewABIndex >= ActionBar->MaxSize){
		NewABIndex = 0;
	}
	AdjustActionBarIndex(NewABIndex);
	
}

void ASurCharacter::MWD(){
	if (!bEnableInput) return;



	if (bIsBuilding) return;
	int32 NewABIndex = ActionBarIndex - 1;
	
	if (NewABIndex < 0){
		NewABIndex = ActionBar->MaxSize - 1;
	}
	AdjustActionBarIndex(NewABIndex);

}

void ASurCharacter::Button1(){
	if (!bEnableInput) return;
	ActionBarIndex = 0;
	EquipItem(ActionBar->Inventory[ActionBarIndex]);
}

void ASurCharacter::Button2(){
	if (!bEnableInput) return;
	ActionBarIndex = 1;
	EquipItem(ActionBar->Inventory[ActionBarIndex]);
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

	// only to local owner
	DOREPLIFETIME_CONDITION(ASurCharacter, PlayerStatus, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ASurCharacter, Inventory, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ASurCharacter, ActionBar, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ASurCharacter, ActionBarIndex, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ASurCharacter, CurrentlyTracedItem, COND_OwnerOnly);

	DOREPLIFETIME(ASurCharacter, CurrentlyEquippedItem);
	DOREPLIFETIME_CONDITION(ASurCharacter, CurrentlyEquippedInventorySlot, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(ASurCharacter, bIsBuilding, COND_OwnerOnly);


}


