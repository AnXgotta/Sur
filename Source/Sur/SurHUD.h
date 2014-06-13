

#pragma once

#include "GameFramework/HUD.h"
#include "SurHUD.generated.h"

/**
 * 
 */
UCLASS()
class ASurHUD : public AHUD
{
	GENERATED_UCLASS_BODY()










	//  BOOLEAN DRAW PROPERTIES

	UPROPERTY()
	bool bDrawInventory;

	UPROPERTY()
		bool bDrawChat;

	UPROPERTY()
		bool bDrawHUD;





};
