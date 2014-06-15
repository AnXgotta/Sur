

#pragma once

#include "Engine.h"
#include "Net/UnrealNetwork.h"


#define TRACE_CHANNEL_ITEM ECC_GameTraceChannel1


#define PRINT_SCREEN(arg0) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White,arg0)

FORCEINLINE void PrintString(UObject* WorldContextObject, const FString& InString, bool bPrintToScreen,  FLinearColor TextColor)
{


	WorldContextObject = GEngine->GetWorldFromContextObject(WorldContextObject, false);
	FString Prefix;
	if (WorldContextObject)
	{
		UWorld *World = WorldContextObject->GetWorld();
		if (World->WorldType == EWorldType::PIE)
		{
			switch (World->GetNetMode())
			{
			case NM_Client:
				Prefix = FString::Printf(TEXT("Client %d: "), GPlayInEditorID - 1);
				break;
			case NM_DedicatedServer:
			case NM_ListenServer:
				Prefix = FString::Printf(TEXT("Server: "));
				break;
			}
		}
	}

	const FString FinalString = Prefix + InString;


	// Also output to the screen, if possible
	if (bPrintToScreen)
	{
		if (GEngine){
			GEngine->AddOnScreenDebugMessage(-1, 10.0, TextColor.ToFColor(true) , FString::Printf(TEXT("%s"),*FinalString));
		}
	}

}