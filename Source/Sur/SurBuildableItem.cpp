

#include "Sur.h"
#include "SurBuildableItem.h"


ASurBuildableItem::ASurBuildableItem(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{


	ItemActionType = EItemAction::Build;

	
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(COLLISION_CHANNEL_BUILDABLE, ECR_Ignore);
	Mesh->SetSimulatePhysics(false);
	Mesh->bGenerateOverlapEvents = true;
	bShowNameOnTrace = false;

	OverlappedObjects = 0;
	bHasBeenBuilt = false;

	AActor::OnActorBeginOverlap.AddDynamic(this, &ASurBuildableItem::OnActorBeginOverlap);
	AActor::OnActorEndOverlap.AddDynamic(this, &ASurBuildableItem::OnActorEndOverlap);
	
}


void ASurBuildableItem::PostInitializeComponents(){
	Super::PostInitializeComponents();

	MI_ObjectMat = Mesh->CreateAndSetMaterialInstanceDynamic(0);

}


//  OVERLAP EVENTS  ################################################################

void ASurBuildableItem::SetBuildableColor(){
	if (!MI_ObjectMat){
		PRINT_SCREEN("Mat Null?");
		return;
	}
	if (OverlappedObjects > 0){		
		PRINT_SCREEN("Red");
		MI_ObjectMat->SetScalarParameterValue(TEXT("RedEmissive"), 0.5f);	
	}
	else{
		PRINT_SCREEN("None");
		MI_ObjectMat->SetScalarParameterValue(TEXT("RedEmissive"), 0.0f);
	}
	
}


void ASurBuildableItem::OnActorBeginOverlap(class AActor* OtherActor){
	if (bHasBeenBuilt) return;
	OverlappedObjects++;
	SetBuildableColor();
}

void ASurBuildableItem::OnActorEndOverlap(class AActor* OtherActor){
	if (bHasBeenBuilt) return;
	OverlappedObjects--;
	SetBuildableColor();
}


//  INTERACTION  #######################################################################




void ASurBuildableItem::OnBeginBuilding(){

}

void ASurBuildableItem::OnEndBuilding(bool Cancelled){

	if (!Cancelled){
		PRINT_SCREEN("END BUILD");
		bHasBeenBuilt = true;
		Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	}
	

}


bool ASurBuildableItem::CanBuild(){	
	return OverlappedObjects == 0;
}

bool ASurBuildableItem::HasBeenBuilt(){
	return bHasBeenBuilt;
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


