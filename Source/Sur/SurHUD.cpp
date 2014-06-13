

#include "Sur.h"
#include "SurHUD.h"


#define BUTTONTYPE_MAIN_RESTART 	1
#define BUTTONTYPE_MAIN_EXIT 		2

#define BUTTONTYPE_CONFIRM_YES 	1
#define BUTTONTYPE_CONFIRM_NO 	2

#define CANVAS_WHITE if(Canvas) Canvas->SetDrawColor(FColor_White);

//Cursor Draw Offset
//		use this to position texture over the point of your cursor, 
//			if the point is not at exactly 0,0
#define CURSOR_DRAW_OFFSET 3


//Static Consts


const FString ASurHUD::S_Title_Main = FString("Sur!");
const FString ASurHUD::S_Title_Confirm = FString("Exit Game?");

const FString ASurHUD::S_Button_Restart = FString("Restart");
const FString ASurHUD::S_Button_Exit = FString("Exit");

// Colors 
const FColor ASurHUD::FColorBlack = FColor(0, 0, 0, 255);
const FColor ASurHUD::FColorRed = FColor(255, 0, 0, 255);
const FColor ASurHUD::FColorYellow = FColor(255, 255, 0, 255);
const FColor ASurHUD::FColorBlue = FColor(0, 0, 255, 255);
const FColor ASurHUD::FColor_White = FColor(255, 255, 255, 255);
// Backgrounds 
const FLinearColor ASurHUD::LC_Black = FLinearColor(0, 0, 0, 1);
const FLinearColor ASurHUD::LC_Pink = FLinearColor(1, 0, 1, 1);
const FLinearColor ASurHUD::LC_Red = FLinearColor(1, 0, 0, 1);
const FLinearColor ASurHUD::LC_Yellow = FLinearColor(1, 1, 0, 1);

ASurHUD::ASurHUD(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{


	bDrawChat = false;
	bDrawInventory = false;
	bDrawHUD = false;

	//States
	ConfirmDialogOpen = false;
	InMainMenu = true;

	//Scale
	GlobalHUDMult = 1;
	DefaultFontScale = 0.7;   //scaling down a size 36 font

}

//Core 
 
void ASurHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
 
	//Establish the PC
	ThePC = GetOwningPlayerController();
 
	//How to get a ref to your custom PC
	//AYourPlayerController* YourChar = Cast<AYourPlayerController>(ThePC);
 
	//How to Get The Character
	//AYourCharacterClass* YourChar = Cast<AYourCharacterClass>(GetOwningPawn());
 
}
 
//===============
// Draw Dialogs
//===============
void ASurHUD::DrawHUD_DrawDialogs()
{
	DrawMainMenu();
	if(ConfirmDialogOpen) DrawConfirm();
}
//Menus
void ASurHUD::DrawMainMenu()
{
	//Background
	DrawMaterialSimple(
		MaterialBackground, 
		10, 10, 
		256, 
		512,
		1.3
	);
 
	//Menu Title
 
	//Draw buttons
	DrawMainMenuButtons();
}
void ASurHUD::DrawConfirm()
{
	//Blue rect with alpha 50%
	DrawSurRect(Canvas->SizeX/2 - 100, Canvas->SizeY/2 - 50,200,100,FLinearColor(0,0,1,0.2333));
 
	//Confirm Title
 
	//Draw buttons
	DrawConfirmButtons();
}
 
//Buttons
void ASurHUD::DrawMainMenuButtons()
{
	//Start Point
	float xStart = 100;
	float yStart = 110;
 
	//Background
	VDrawTile(ButtonBackground,xStart,yStart,150,80,FColor(255,255,255,120)); //alpha 120/255
 
	//Text
	DrawSurText(
		UIFont,"Restart",xStart+30,yStart+20,
		LC_Black, DefaultFontScale,
		true,LC_Red
	);
 
	//Struct
	//Add Button If Necessary
	//		could be cleared and need refreshing if using a different menu
	//			clear buttons with ButtonsMain.Empty()
	if (ButtonsMain.Num() < 1 )
	{
		FSurButtonStruct newButton = FSurButtonStruct();
		newButton.type 			= BUTTONTYPE_MAIN_RESTART;
		newButton.toolTip		= "Restart the Game!";	
		newButton.minX 			= xStart;
		newButton.maxX 			= xStart + 150;		
		newButton.minY 			= yStart;
		newButton.maxY 			= yStart + 80;
 
		//Add to correct array
		ButtonsMain.Add(newButton);
	}
 
 
	xStart = 100;
	yStart = 410;
 
	VDrawTile(ButtonBackground,xStart,yStart,150,80,FColor(255,255,255,120)); //alpha 120/255
 
	//Text
	DrawSurText(
		UIFont,"Exit",xStart+55,yStart+20,
		LC_Black, DefaultFontScale,
		true,LC_Red
	);
 
	if (ButtonsMain.Num() < 2 )
	{
		FSurButtonStruct newButton = FSurButtonStruct();
		newButton.type 			= BUTTONTYPE_MAIN_EXIT;
		newButton.toolTip			= "Exit the Game!";	
		newButton.minX 			= xStart;
		newButton.maxX 			= xStart + 150;		
		newButton.minY 			= yStart;
		newButton.maxY 			= yStart + 80;
 
		//Add to correct array
		ButtonsMain.Add(newButton);
	}
}
void ASurHUD::DrawConfirmButtons()
{
	float xStart = Canvas->SizeX/2 - 100;
	float yStart = Canvas->SizeY/2 - 40;
 
	//Highlighted?
	if(ActiveButton_Type == BUTTONTYPE_CONFIRM_YES ) ColorPtr = &LC_Pink;
	else ColorPtr = &LC_Yellow;
 
	//Text
	DrawSurText(
		UIFont,"Yes",xStart+30,yStart+20,
		*ColorPtr, DefaultFontScale,
		true
	);
 
	if (ButtonsConfirm.Num() < 1 )
	{
		FSurButtonStruct newButton = FSurButtonStruct();
		newButton.type 			= BUTTONTYPE_CONFIRM_YES ;
		newButton.toolTip			= "";	
		newButton.minX 			= xStart;
		newButton.maxX 			= xStart + 75;		
		newButton.minY 			= yStart + 20;
		newButton.maxY 			= yStart + 60;
 
		//could use GetTextSize to streamline this
 
		//Add to correct array
		ButtonsConfirm.Add(newButton);
	}
 
	xStart = Canvas->SizeX/2 + 20;
	yStart = Canvas->SizeY/2 - 40;
 
	//Highlighted?
	if(ActiveButton_Type == BUTTONTYPE_CONFIRM_NO) ColorPtr = &LC_Black;
	else ColorPtr = &LC_Yellow;
 
	//Text
	DrawSurText(
		UIFont,"No",xStart+30,yStart+20,
		*ColorPtr, DefaultFontScale,
		true
	);
 
	if (ButtonsConfirm.Num() < 2 )
	{
		FSurButtonStruct newButton = FSurButtonStruct();
		newButton.type 			= BUTTONTYPE_CONFIRM_NO;
		newButton.toolTip			= "";	
		newButton.minX 			= xStart;
		newButton.maxX 			= xStart + 75;		
		newButton.minY 			= yStart + 20;
		newButton.maxY 			= yStart + 60;
 
		//could use GetTextSize to streamline this
 
		//Add to correct array
		ButtonsConfirm.Add(newButton);
	}
}
 
//===============
// Cursor In Buttons
//===============
int32 ASurHUD::CheckCursorInButton(const TArray<FSurButtonStruct>& ButtonArray)
{
	for(int32 b = 0; b < ButtonArray.Num(); b++)
	{
		CurCheckButton = &ButtonArray[b];
 
		//check cursor in bounds
		if (CurCheckButton->minX <= MouseLocation.X && MouseLocation.X <= CurCheckButton->maxX &&
			CurCheckButton->minY <= MouseLocation.Y && MouseLocation.Y <= CurCheckButton->maxY )
		{
 
			//Active Button Type
			ActiveButton_Type = CurCheckButton->type; 
 
			//Tool Tip
			ActiveButton_Tip = CurCheckButton->toolTip; 
 
			//Change Cursor
			CursorHoveringInButton = true;
 
			//Mouse Clicked?
			if (ThePC->WasInputKeyJustPressed(EKeys::LeftMouseButton))
			{
				return ActiveButton_Type;
				//~~
				//no need to check rest of buttons
			}
		}
	}
 
	//No Click Occurred This Tick
	return -1;	
}
 
//Check Confirm
void ASurHUD::CheckCursorInButtonsConfirm()
{
	//Check Confirm Buttons
	ClickedButtonType = CheckCursorInButton(ButtonsConfirm); //fills global ActiveButton_Type
 
	if(ClickedButtonType == BUTTONTYPE_CONFIRM_YES )
	{
		ThePC->ConsoleCommand("Exit");
		return;
	}
	if(ClickedButtonType == BUTTONTYPE_CONFIRM_NO)
	{
		ConfirmDialogOpen = false;
		ButtonsConfirm.Empty(); //Buttons not needed anymore
		return;
	}
}
 
//Check Buttons
void ASurHUD::CheckCursorInButtonsMain()
{
	//Check Confirm Buttons
	ClickedButtonType = CheckCursorInButton(ButtonsMain);
 
	if(ClickedButtonType == BUTTONTYPE_MAIN_RESTART )
	{
		ThePC->ConsoleCommand("RestartLevel");
		return;
	}
	if(ClickedButtonType == BUTTONTYPE_MAIN_EXIT)
	{
		ConfirmDialogOpen = true;
		return;
	}
}
void ASurHUD::DrawHUD_CheckCursorInButtons()
{
	if(ConfirmDialogOpen)
	{
		CheckCursorInButtonsConfirm();
 
		//Take Focus Away From All Other buttons
		return; 
		//~
	}
 
	//Main
	CheckCursorInButtonsMain();
}
 
void ASurHUD::DrawToolTip()
{
	//if mouse is too far to right, draw from left instead
	float xStart = MouseLocation.X + 150;
	float yStart = MouseLocation.Y + 5;
 
	//out vars
	float RV_xLength; 
	float RV_yLength;
	//Text Size
	GetTextSize(
		ActiveButton_Tip, 
		RV_xLength, 
		RV_yLength, 
		UIFont,
		DefaultFontScale * 2
	);
 
	// Decide Draw to Left or to the Right 
 
	//Draw to the Left
	if (xStart + RV_xLength >= Canvas->SizeX - 40)
	{
		xStart -= 150 + 140 + 64 + RV_xLength;
 
		//If Text is too long, bring it closer to the cursor
		if(xStart < 33 ) xStart = 33;
	}
 
	//Background
	DrawSurRect(
		xStart, yStart, 
		RV_xLength + 70, 
		80, 
		FLinearColor(0, 0, 1, 0.7) //alpha 0.7
	);
 
	//Tool Tip
	DrawText(
		ActiveButton_Tip, 
		LC_Pink,
		xStart + 32, yStart + 20,
		UIFont,
		DefaultFontScale * 2,			
		false		//scale position of message with HUD scale
	);
}
void ASurHUD::DrawHUD_DrawCursor()
{
	//Tool Tip
	if(ActiveButton_Tip != "") DrawToolTip();
 
	//Cursor Hovering in a Button?
	if (CursorHoveringInButton)
	{
		//pointer tex found?
		if (!CursorHoveringButton) return;
		DrawFullSizeTile(CursorHoveringButton, MouseLocation.X - CURSOR_DRAW_OFFSET, MouseLocation.Y - CURSOR_DRAW_OFFSET, FColor_White );
	}
 
	else
	{
		//cursor tex found?
		if(!CursorMain) return;
		DrawFullSizeTile(CursorMain, MouseLocation.X - CURSOR_DRAW_OFFSET, MouseLocation.Y - CURSOR_DRAW_OFFSET, FColor_White );
	}
}
 
void ASurHUD::PlayerInputChecks()
{
	//check out this tutorial of mine for a list of all EKeys::
	//http://forums.epicgames.com/threads/972861-Tutorials-C-for-UE4-Code-Samples-gt-gt-New-Video-Freeze-Render-When-Tabbed-Out?p=31660286&viewfull=1#post31660286
 
	if(ThePC->WasInputKeyJustPressed(EKeys::Escape))
	{
		SetCursorMoveOnly(false);
		return;
	}
	if(ThePC->WasInputKeyJustPressed(EKeys::F))
	{
		SetCursorMoveOnly(!ThePC->IsLookInputIgnored());
		return;
	}
	if(ThePC->WasInputKeyJustPressed(EKeys::H))
	{
		DontDrawHUD = !DontDrawHUD;
		return;
	}
 
	//Confirm
	if(ConfirmDialogOpen)
	{
		if(ThePC->WasInputKeyJustPressed(EKeys::Y))
		{
			ThePC->ConsoleCommand("Exit"); 
			//could replace with function based on confirm context
 
			return;
		}
		if(ThePC->WasInputKeyJustPressed(EKeys::N))
		{
			ConfirmDialogOpen = false;
			ButtonsConfirm.Empty(); //Buttons not needed anymore
			//Cancel Confirm
 
			return;
		}
	}
}
 
void ASurHUD::DrawHUD_Reset()
{
	ActiveButton_Type 		= -1;
	ActiveButton_Tip 		= "";
	CursorHoveringInButton 	= false;
}
 
void ASurHUD::DrawHUD()
{
	//==============================
	//==============================
	//==============================
	//Have PC for Input Checks and Mouse Cursor?
	if(!ThePC)
	{
		//Attempt to Reacquire PC
		ThePC = GetOwningPlayerController();
 
		//Could Not Obtain PC
		if(!ThePC) return;
		//~~
	}
 
	//Multiplayer Safety Check
	if(!ThePC->PlayerInput) return; //not valid for first seconds of a multiplayer client
	//~~
	//==============================
	//==============================
	//==============================
 
	//Player Input
	PlayerInputChecks();
 
	//Draw HUD?
	if(DontDrawHUD) return;
	//~~
 
	//Super
	Super::DrawHUD();
 
	//No Canvas?
	if(!Canvas) return;
	//
 
	//Reset States
	DrawHUD_Reset();
 
	//================
	//Get New Mouse Position
	//================
	ThePC->GetMousePosition(MouseLocation.X,MouseLocation.Y);
 
	//Cursor In Buttons
	DrawHUD_CheckCursorInButtons();
 
	//Draw Dialogs
	//DrawHUD_DrawDialogs();
 
	//### Do Last ###
	//Draw Cursor
	//DrawHUD_DrawCursor();
 
	//Debugging Info
	//ThePC->ClientMessage("HUD Loop Completed!");
}