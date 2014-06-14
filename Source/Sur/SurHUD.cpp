

#include "Sur.h"
#include "SurCharacter.h"
#include "SurHUD.h"


#define BUTTONTYPE_INVENTORY_SLOT 	1
#define BUTTONTYPE_ACTIONBAR_SLOT	2


#define CANVAS_WHITE if(Canvas) Canvas->SetDrawColor(FColor_White);

//Cursor Draw Offset
//		use this to position texture over the point of your cursor, 
//			if the point is not at exactly 0,0
#define CURSOR_DRAW_OFFSET 3


//Static Consts



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
const FLinearColor ASurHUD::LC_Transparent = FLinearColor(0, 0, 0, 0);

ASurHUD::ASurHUD(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{


	bDrawChat = false;
	bDrawInventory = false;
	bDrawHUD = true;
	bIsHoldingLMB = false;
	bIsDraggingButton = false;

	//Scale
	GlobalHUDMult = 1;
	DefaultFontScale = 0.7;   //scaling down a size 36 font
	ScreenHWRatio = 1.0f;
	ScreenDivisor = 16.0f;
	ScreenRes = FVector2D(0.0f, 0.0f);
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
 


//  Is cursor in a button
int32 ASurHUD::CheckCursorInButton(const TArray<FSurButtonStruct>& ButtonArray){

	for(int32 b = 0; b < ButtonArray.Num(); b++)	{
		CurCheckButton = &ButtonArray[b];
 
		//check cursor in bounds
		if (CurCheckButton->minX <= MouseLocation.X && MouseLocation.X <= CurCheckButton->maxX &&
			CurCheckButton->minY <= MouseLocation.Y && MouseLocation.Y <= CurCheckButton->maxY ){
 
			//Active Button Type
			ActiveButton_Type = CurCheckButton->type; 
 
			//Tool Tip
			ActiveButton_Tip = CurCheckButton->toolTip; 

			if (bIsDraggingButton){
				if (ThePC->WasInputKeyJustReleased(EKeys::LeftMouseButton)){
					bIsDraggingButton = false;
					PRINT_SCREEN("Drag Released");
					// drop in same slot, or no button being dragged - return
					if (!CurInventorySlot || !CurDraggedButton || CurDraggedButton == CurCheckButton) return -1;

					ASurCharacter* TheChar = Cast<ASurCharacter >(GetOwningPawn());
					if (!TheChar) return -1;

					switch (CurCheckButton->type){
					case BUTTONTYPE_ACTIONBAR_SLOT:
						PRINT_SCREEN("Dropped into Action Slot");
						TheChar->HandleInventoryUITransaction(CurInventorySlot, TheChar->ActionBar->Inventory[CurCheckButton->index]);
						break;
					case BUTTONTYPE_INVENTORY_SLOT:
						PRINT_SCREEN("Dropped into Inven Slot");
						TheChar->HandleInventoryUITransaction(CurInventorySlot, TheChar->Inventory->Inventory[CurCheckButton->index]);
						break;
					default:
						break;
					}

					CurInventorySlot = NULL;
					CurDraggedButton = NULL;

				}
			}else if (ThePC->WasInputKeyJustPressed(EKeys::LeftMouseButton)){
				PRINT_SCREEN("Drag/Click Started");
				ASurCharacter* TheChar = Cast<ASurCharacter >(GetOwningPawn());
				if (!TheChar) return -1;

				switch (CurCheckButton->type){
				case BUTTONTYPE_ACTIONBAR_SLOT:
					PRINT_SCREEN("Picked Action Slot");
					if (TheChar->ActionBar->Inventory[CurCheckButton->index]->IsSlotEmpty()) return -1;
					CurDraggedButton = CurCheckButton;
					CurInventorySlot = TheChar->ActionBar->Inventory[CurCheckButton->index];
					break;
				case BUTTONTYPE_INVENTORY_SLOT:
					PRINT_SCREEN("Picked Inven Slot");
					CurDraggedButton = CurCheckButton;
					if (TheChar->Inventory->Inventory[CurCheckButton->index]->IsSlotEmpty()) return -1;
					CurInventorySlot = TheChar->Inventory->Inventory[CurCheckButton->index];
					break;
				default:
					break;
				}

				bIsDraggingButton = true;


			}
		}
	}
 
	//No Click Occurred This Tick
	return -1;	
}


void ASurHUD::DrawActionBar(){
	ASurCharacter* TheChar = Cast<ASurCharacter >(GetOwningPawn());
	if (!TheChar) return;
	USurInventory* ActionBar = TheChar->ActionBar;
	if (!ActionBar) return;

	float Y = (ScreenDivisor / ScreenHWRatio);
	float SY = ScreenRes.Y / Y;

	for (int i = 0; i < ActionBar->Inventory.Num(); i++){
		FSurButtonStruct NewSlot;
		NewSlot.type = BUTTONTYPE_ACTIONBAR_SLOT;
		NewSlot.minX = ( 4.0f * ScreenRes.X / ScreenDivisor) + ((float)i * ScreenRes.X / ScreenDivisor);
		NewSlot.maxX = (NewSlot.minX + (ScreenRes.X / ScreenDivisor));
		NewSlot.minY = ((0.875 * Y ) * SY);
		NewSlot.maxY = (NewSlot.minY + SY);
		NewSlot.index = i;

		VDrawTile(InventorySlotBackground, NewSlot.minX, NewSlot.minY, NewSlot.maxX - NewSlot.minX, NewSlot.maxY - NewSlot.minY, FColorBlack);

		if (!ActionBar->Inventory[i]->IsSlotEmpty()){
			VDrawTile(ActionBar->Inventory[i]->GetItemDisplayTexture(), NewSlot.minX, NewSlot.minY, NewSlot.maxX - NewSlot.minX, NewSlot.maxY - NewSlot.minY, FColorRed);
			DrawSurText(UIFont, FString::FromInt(ActionBar->Inventory[i]->NumberItemsStacked), NewSlot.minX, NewSlot.minY, FColorRed, 0.5f);
			NewSlot.toolTip = ActionBar->Inventory[i]->GetItemName().ToString();
		}

		if (i == TheChar->ActionBarIndex){
			DrawSurText(UIFont, "E", NewSlot.minX + ((NewSlot.maxX - NewSlot.minX) / 2.0f), NewSlot.minY + ((NewSlot.maxY - NewSlot.minY) / 2.0f), FColorRed, 0.5f);
		}

		ButtonsActionBar.Add(NewSlot);
	}

	CheckCursorInButton(ButtonsActionBar);

}

void ASurHUD::DrawInventory(){
	ASurCharacter* TheChar = Cast<ASurCharacter >(GetOwningPawn());
	if (!TheChar) return;
	USurInventory* Inventory = TheChar->Inventory;
	if (!Inventory) return;

	

	float posX = 0.0f;
	float posY = 0.0f;

	int slotIndex = 0;

	float Y = (ScreenDivisor / ScreenHWRatio);
	float SY = ScreenRes.Y / Y;

	for (int i = 0; i < Inventory->Inventory.Num() / 8; i++){
		for (int j = 0; j < Inventory->Inventory.Num() / 4; j++){
			FSurButtonStruct NewSlot;
			NewSlot.type = BUTTONTYPE_INVENTORY_SLOT;
			NewSlot.minX = (4.0f * ScreenRes.X / ScreenDivisor) + ((float)j * ScreenRes.X / ScreenDivisor);
			NewSlot.maxX = (NewSlot.minX + (ScreenRes.X / ScreenDivisor));
			NewSlot.minY = ((0.125 * Y) * SY) + (i * SY);
			NewSlot.maxY = (NewSlot.minY + SY);
			NewSlot.index = slotIndex;

			posX += 1.0f;

			VDrawTile(InventorySlotBackground, NewSlot.minX, NewSlot.minY, NewSlot.maxX - NewSlot.minX, NewSlot.maxY - NewSlot.minY, FColorBlack);

			if (!Inventory->Inventory[slotIndex]->IsSlotEmpty()){
				VDrawTile(Inventory->Inventory[slotIndex]->GetItemDisplayTexture(), NewSlot.minX, NewSlot.minY, NewSlot.maxX - NewSlot.minX, NewSlot.maxY - NewSlot.minY, FColorRed);
				DrawSurText(UIFont, FString::FromInt(Inventory->Inventory[slotIndex]->NumberItemsStacked), NewSlot.minX, NewSlot.minY, FColorRed, 0.5f);
				NewSlot.toolTip = Inventory->Inventory[slotIndex]->GetItemName().ToString();
			}

			ButtonsInventory.Add(NewSlot);
			++slotIndex;
		}
		
	}
	DrawCursor();
	CheckCursorInButton(ButtonsInventory);
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

void ASurHUD::DrawCursor(){
	//Tool Tip
	//if(ActiveButton_Tip != "") DrawToolTip();
 

	//cursor tex found?
	if(!CursorMain) return;

	float Y = (ScreenDivisor / ScreenHWRatio);
	float SY = ScreenRes.Y / Y;
	float CursorHeight = ((0.05 * Y) * SY);
	float CursorWidth = (0.5f * ScreenRes.X / ScreenDivisor);

	if (bIsDraggingButton && CurInventorySlot){
		float TextureHeight = ((0.1 * Y) * SY);
		float TextureWidth = (1.0f * ScreenRes.X / ScreenDivisor);
		VDrawTile(CurInventorySlot->ItemDisplayTexture, MouseLocation.X - (0.5f * TextureWidth), MouseLocation.Y - (0.5f * TextureHeight), TextureWidth, TextureHeight, FColorBlack);
	}

	
	VDrawTile(CursorMain, MouseLocation.X - (0.5f * CursorWidth), MouseLocation.Y - (0.5f * CursorHeight), CursorWidth, CursorHeight, FColorBlack);
	
}
 
void ASurHUD::DrawCrosshair(){
	float Y = (ScreenDivisor / ScreenHWRatio);
	float SY = ScreenRes.Y / Y;
	float CursorHeight = ((0.05 * Y) * SY);
	float CursorWidth = (0.5f * ScreenRes.X / ScreenDivisor);
	VDrawTile(CursorMain, (ScreenRes.X/2.0f) - (0.5f * CursorWidth), (ScreenRes.Y/2.0f) - (0.5f * CursorHeight), CursorWidth, CursorHeight, FColorBlack);
}

void ASurHUD::PlayerInputChecks(){

	if(ThePC->WasInputKeyJustPressed(EKeys::I))	{
		ASurCharacter* MyChar = Cast<ASurCharacter >(GetOwningPawn());
		if (MyChar){
			if (!MyChar->CanOpenInventoryUI()) return;
			MyChar->bEnableInput = !MyChar->bEnableInput;
		}
		SetCursorMoveOnly(!ThePC->IsLookInputIgnored());
		bDrawInventory = !bDrawInventory;
		return;
	}

	if(ThePC->WasInputKeyJustPressed(EKeys::H))	{
		bDrawHUD = !bDrawHUD;
		return;
	}
 
	
}
 
void ASurHUD::ResetStateInfo(){
	ActiveButton_Type 		= -1;
	ActiveButton_Tip 		= "";
}
 
void ASurHUD::SetScreenResolution(float ScreenX, float ScreenY){
	ScreenRes.X = ScreenX;
	ScreenRes.Y = ScreenY;
	ScreenHWRatio = ScreenRes.X / ScreenRes.Y;
}

void ASurHUD::DrawHUD(){

	//Have PC for Input Checks and Mouse Cursor?
	if(!ThePC){
		//Attempt to Reacquire PC
		ThePC = GetOwningPlayerController();
 
		//Could Not Obtain PC
		if(!ThePC) return;
		//~~
	}

	//Multiplayer Safety Check
	if(!ThePC->PlayerInput) return; //not valid for first seconds of a multiplayer client
 
	//Player Input
	PlayerInputChecks();


 
	
 
	//Super
	Super::DrawHUD();
 
	//No Canvas?
	if(!Canvas) return;
	
	//Draw HUD?
	if (bDrawHUD){
		DrawCrosshair();
		DrawActionBar();
	}

	if (bDrawInventory){
		DrawInventory();
	}
 
	//Reset States
	ResetStateInfo();
 
	

	// get mouse position
	ThePC->GetMousePosition(MouseLocation.X,MouseLocation.Y);
 
	//Cursor In Buttons
	//DrawHUD_CheckCursorInButtons();

 
	//### Do Last ###
	//Draw Cursor
	//DrawHUD_DrawCursor();
 
	//Debugging Info
	//PRINT_SCREEN("HUD LOOP");
}