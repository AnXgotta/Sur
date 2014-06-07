

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
	Mesh->BodyInstance.SetObjectType(ECC_WorldDynamic);
	Mesh->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->BodyInstance.SetResponseToAllChannels(ECR_Block);
	Mesh->SetHiddenInGame(false);


	bReplicateMovement = true;
	bReplicates = true;
}

void ASurItem::BeginPlay(){
	Super::BeginPlay();
	if (Mesh){
		Mesh->SetSimulatePhysics(true);
		Mesh->WakeRigidBody();
	}
}

FName ASurItem::GetUIName(){
	return ItemUIData.UIName;
}


//  INVENTORY  ##########################################################################

void ASurItem::ItemPickedUp(){
	SetActorHiddenInGame(true);
	Mesh->DestroyComponent();
	Destroy();
}


void ASurItem::ItemDropped(FVector Trajectory){
	Mesh->AddForce(Trajectory * 100.0f);
}
