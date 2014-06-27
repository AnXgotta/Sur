

#include "Sur.h"
#include "SurBuildableItem.h"


ASurBuildableItem::ASurBuildableItem(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{


	ItemActionType = EItemAction::Build;

	Mesh->BodyInstance.SetObjectType(ECC_WorldStatic);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(COLLISION_CHANNEL_BUILDABLE, ECR_Ignore);
	Mesh->SetSimulatePhysics(false);
	Mesh->bGenerateOverlapEvents = true;
	bShowNameOnTrace = false;

	bInBuildablePosition = true;
	bHasBeenBuilt = false;

	AActor::OnActorBeginOverlap.AddDynamic(this, &ASurBuildableItem::OnActorBeginOverlap);
	AActor::OnActorEndOverlap.AddDynamic(this, &ASurBuildableItem::OnActorEndOverlap);
	
}


void ASurBuildableItem::PostInitializeComponents(){
	Super::PostInitializeComponents();

	MI_ObjectMat = Mesh->CreateAndSetMaterialInstanceDynamic(0);

}


//  OVERLAP EVENTS  ################################################################

void ASurBuildableItem::SetInBuildablePosition(bool bCanBuild){
	bInBuildablePosition = bCanBuild;
}

void ASurBuildableItem::SetBuildableColor(){
	if (!MI_ObjectMat){
		PRINT_SCREEN("Mat Null?");
		return;
	}
	if (!bInBuildablePosition){
		
		MI_ObjectMat->SetScalarParameterValue(TEXT("RedEmissive"), 0.5f);	
	}
	else{
		MI_ObjectMat->SetScalarParameterValue(TEXT("RedEmissive"), 0.0f);
	}
	
}


void ASurBuildableItem::OnActorBeginOverlap(class AActor* OtherActor){
	if (bHasBeenBuilt || !bInBuildablePosition) return;
	bInBuildablePosition = false;
	SetBuildableColor();
}

void ASurBuildableItem::OnActorEndOverlap(class AActor* OtherActor){
	if (bHasBeenBuilt || bInBuildablePosition) return;
	bInBuildablePosition = true;
	SetBuildableColor();
}


//  INTERACTION  #######################################################################




void ASurBuildableItem::OnBeginBuilding(){

}

void ASurBuildableItem::OnEndBuilding(bool Cancelled){

	if (!Cancelled){
		bHasBeenBuilt = true;
		Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	}
	

}

bool ASurBuildableItem::CanBuild(){
	
	return bInBuildablePosition;
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

