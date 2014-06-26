

#include "Sur.h"
#include "SurBuildableItem.h"


ASurBuildableItem::ASurBuildableItem(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	ItemActionType = EItemAction::Build;

	Mesh->BodyInstance.SetObjectType(ECC_WorldStatic);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetSimulatePhysics(false);
	Mesh->bGenerateOverlapEvents = true;


	bShowNameOnTrace = false;

}





//  OVERLAP EVENTS  ################################################################


void ASurBuildableItem::OnBeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	PRINT_SCREEN("OVERLAP BEGIN");
}

void ASurBuildableItem::OnEndOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	PRINT_SCREEN("OVERLAP END");
}


//  INTERACTION  #######################################################################


void ASurBuildableItem::OnBeginBuilding(){

}

void ASurBuildableItem::OnEndBuilding(bool Cancelled){


}



void ASurBuildableItem::OnItemEquipped(){
	Super::OnItemEquipped();
	
}

void ASurBuildableItem::OnItemUnEquipped(){
	Super::OnItemUnEquipped();
}

void ASurBuildableItem::OnUseItem(){
	Super::OnUseItem();
	//Mesh->SetCollisionResponseToAllChannels(ECC_);
}

