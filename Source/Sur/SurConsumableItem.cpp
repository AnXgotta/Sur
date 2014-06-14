

#include "Sur.h"
#include "SurConsumableItem.h"


ASurConsumableItem::ASurConsumableItem(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	ItemActionType = EItemAction::Consume;


}






//  INTERACTION  #######################################################################

void ASurConsumableItem::OnItemEquipped(){
	Super::OnItemEquipped();

}

void ASurConsumableItem::OnItemUnEquipped(){
	Super::OnItemUnEquipped();

}

void ASurConsumableItem::OnUseItem(){
	Super::OnUseItem();
	
}

