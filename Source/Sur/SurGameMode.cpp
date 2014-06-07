

#include "Sur.h"
#include "SurGameMode.h"
#include "SurPlayerController.h"

ASurGameMode::ASurGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PlayerControllerClass = ASurPlayerController::StaticClass();
}


