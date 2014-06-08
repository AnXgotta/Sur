

#include "Sur.h"
#include "SurGameMode.h"
#include "SurPlayerController.h"

ASurGameMode::ASurGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerPawnObject(TEXT("Blueprint'/Game/Character/Blueprint/BP_SurCharacter.BP_SurCharacter'"));
	if (PlayerPawnObject.Object != NULL)	{
		DefaultPawnClass = (UClass*)PlayerPawnObject.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerHUDObject(TEXT("Blueprint'/Game/HUD/BP_HUD_Test.BP_HUD_Test'"));
	if (PlayerPawnObject.Object != NULL)	{
		HUDClass = (UClass*)PlayerHUDObject.Object->GeneratedClass;
	}

	PlayerControllerClass = ASurPlayerController::StaticClass();
}


