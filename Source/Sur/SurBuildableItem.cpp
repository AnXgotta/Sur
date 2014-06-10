

#include "Sur.h"
#include "SurBuildableItem.h"


ASurBuildableItem::ASurBuildableItem(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}










//  INTERACTION  #######################################################################

void ASurBuildableItem::OnItemEquipped(){
	Super::OnItemEquipped();
}

void ASurBuildableItem::OnItemUnEquipped(){
	Super::OnItemUnEquipped();
}

void ASurBuildableItem::OnUseItem(){
	Super::OnUseItem();
}

