

#include "Sur.h"
#include "SurItem.h"


ASurItem::ASurItem(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("ItemMesh"));
	RootComponent = Mesh;
	Mesh->SetOnlyOwnerSee(false);
	Mesh->bCastDynamicShadow = true;
	Mesh->CastShadow = true;
	//Mesh->BodyInstance.SetObjectType(ECC_WorldDynamic);
	//Mesh->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//Mesh->BodyInstance.SetResponseToAllChannels(ECR_Block);
	Mesh->BodyInstance.SetObjectType(ECC_WorldDynamic);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->BodyInstance.SetResponseToAllChannels(ECR_Block);
	Mesh->SetHiddenInGame(false);
	Mesh->SetSimulatePhysics(true);
	Mesh->WakeRigidBody();

	bReplicateMovement = true;
	bReplicates = true;

	/*

	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint((TCHAR*)&BlueprintPath);
	if (ItemBlueprint.Object){
		SurItemBlueprint = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}
	*/

}



void ASurItem::BeginPlay(){
	Super::BeginPlay();

}

//  HELPERS  ###########################################################################

TEnumAsByte<EItemAction::Type> ASurItem::GetItemActionType(){
	return ItemActionType;
}


//  INVENTORY  ##########################################################################

void ASurItem::ItemPickedUp(){
	SetActorHiddenInGame(true);
	//Mesh->DestroyComponent();
	Destroy();
}

void ASurItem::ItemDropped(FVector Trajectory){
	if (Mesh){
		Mesh->AddImpulse(Trajectory * 10000.0f);
	}
	
}


//  USAGE  #############################################################################


void ASurItem::OnItemEquipped(){
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	PRINT_SCREEN("ASurItem [OnItemEquipped] Called");
}

void ASurItem::OnItemUnEquipped(){
	SetActorHiddenInGame(true);
	Mesh->AttachParent = NULL;
	Destroy();
	PRINT_SCREEN("ASurItem [OnItemUnEquipped] Called");
}

void ASurItem::OnUseItem(){
	
}