

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

	PlayerControllerClass = ASurPlayerController::StaticClass();
}


