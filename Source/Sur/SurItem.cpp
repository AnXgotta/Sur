

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

	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/Items/Blueprints/BP_ItemTest.BP_ItemTest'"));
	if (ItemBlueprint.Object){
		SurItemBlueprint = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}

}

void ASurItem::BeginPlay(){
	Super::BeginPlay();
	if (Mesh){
		Mesh->SetSimulatePhysics(true);
		Mesh->WakeRigidBody();
	}

}



FName ASurItem::GetUIName(){
	return ItemDataUI.UIName;
}



//  INVENTORY  ##########################################################################

void ASurItem::ItemPickedUp(){
	SetActorHiddenInGame(true);
	Mesh->DestroyComponent();
	Destroy();
}

void ASurItem::ItemDropped(FVector Trajectory){
	Mesh->AddImpulse(Trajectory * 10000.0f);
}

