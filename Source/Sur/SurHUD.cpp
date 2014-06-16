

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
	
	float TextureSize = 0.0625f * ScreenRes.X;
	for (int i = 0; i < ActionBar->Inventory.Num(); i++){
		FSurButtonStruct NewSlot;
		NewSlot.type = BUTTONTYPE_ACTIONBAR_SLOT;
		NewSlot.minX = (0.25f * ScreenRes.X) + (i * TextureSize);
		NewSlot.maxX = NewSlot.minX + TextureSize;
		NewSlot.minY = 0.85f * ScreenRes.Y;
		NewSlot.maxY = NewSlot.minY + TextureSize;
		NewSlot.index = i;


		if (i == TheChar->ActionBarIndex){
			VDrawTile(ActiveInventorySlotBackground, NewSlot.minX, NewSlot.minY, NewSlot.maxX - NewSlot.minX, NewSlot.maxY - NewSlot.minY, FColorBlack);
		}else{
			VDrawTile(InventorySlotBackground, NewSlot.minX, NewSlot.minY, NewSlot.maxX - NewSlot.minX, NewSlot.maxY - NewSlot.minY, FColorBlack);
		}

		if (!ActionBar->Inventory[i]->IsSlotEmpty()){
			VDrawTile(ActionBar->Inventory[i]->GetItemDisplayTexture(), NewSlot.minX, NewSlot.minY, NewSlot.maxX - NewSlot.minX, NewSlot.maxY - NewSlot.minY, FColorRed);
			DrawSurText(UIFont, FString::FromInt(ActionBar->Inventory[i]->NumberItemsStacked), NewSlot.minX, NewSlot.minY, FColorRed, 0.5f);
			NewSlot.toolTip = ActionBar->Inventory[i]->GetItemName().ToString();
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

	int slotIndex = 0;

	float TextureSize = 0.0625f * ScreenRes.X;
	
	for (int i = 0; i < Inventory->Inventory.Num() / 8; i++){
		for (int j = 0; j < Inventory->Inventory.Num() / 4; j++){
			FSurButtonStruct NewSlot;
			NewSlot.type = BUTTONTYPE_INVENTORY_SLOT;
			NewSlot.minX = (0.250f * ScreenRes.X) + (j * TextureSize);
			NewSlot.maxX = NewSlot.minX + TextureSize;
			NewSlot.minY = (0.20f * ScreenRes.Y) + (i * TextureSize);
			NewSlot.maxY = NewSlot.minY + TextureSize;
			NewSlot.index = slotIndex;

			VDrawTile(InventorySlotBackground, NewSlot.minX, NewSlot.minY, NewSlot.maxX - NewSlot.minX, NewSlot.maxY - NewSlot.minY, FColorBlack);

			if (!Inventory->Inventory[slotIndex]->IsSlotEmpty()){
				VDrawTile(Inventory->Inventory[slotIndex]->GetItemDisplayTexture(), NewSlot.minX, NewSlot.minY, NewSlot.maxX - NewSlot.minX, NewSlot.maxY - NewSlot.minY, FColorRed);
				DrawSurText(UIFont, FString::FromInt(Inventory->Inventory[slotIndex]->NumberItemsStacked), NewSlot.minX, NewSlot.minY, FColorRed, 0.5f);
				NewSlot.toolTip = Inventory->Inventory[slotIndex]->GetItemName().ToString();
			}

			ButtonsInventory.Add(NewSlot);
			slotIndex++;
		}
		
	}
	DrawCursor();
	CheckCursorInButton(ButtonsInventory);
}
 
void ASurHUD::DrawPlayerStatus(){
	ASurCharacter* TheChar = Cast<ASurCharacter >(GetOwningPawn());
	if (!TheChar) return;

	float posX = 0.1f * ScreenRes.X;	
	float posY = 0.05f * ScreenRes.Y;
	float ex = posY;

	DrawSurText(UIFont, FString::Printf(TEXT("Health_%.2f"), TheChar->PlayerStatus.Health), posX, posY, FColorBlue, 0.5f);
	posY += ex;
	DrawSurText(UIFont, FString::Printf(TEXT("Stamina_%.2f"), TheChar->PlayerStatus.Stamina), posX, posY, FColorBlue, 0.5f);
	posY += ex;
	DrawSurText(UIFont, FString::Printf(TEXT("Hunger_%.2f"), TheChar->PlayerStatus.Hunger), posX, posY, FColorBlue, 0.5f);
	posY += ex;
	DrawSurText(UIFont, FString::Printf(TEXT("Thirst_%.2f"), TheChar->PlayerStatus.Thirst), posX, posY, FColorBlue, 0.5f);

}
 
void ASurHUD::DrawCurrentlyTracedItem(){
	ASurCharacter* TheChar = Cast<ASurCharacter >(GetOwningPawn());
	if (!TheChar) return;
	if (!TheChar->CurrentlyTracedItem) return;

	FVector UIPos = Project(TheChar->CurrentlyTracedItem->GetActorLocation());
	DrawSurText(UIFont, TheChar->CurrentlyTracedItem->UIName.ToString(), UIPos.X, UIPos.Y, FColorYellow, 0.4f);


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

	float CursorDim = 0.025f * ScreenRes.X;
	if (bIsDraggingButton && CurInventorySlot){	
		float TextureDim = 0.0625f * ScreenRes.X;
		VDrawTile(CurInventorySlot->ItemDisplayTexture, MouseLocation.X - (0.5f * TextureDim), MouseLocation.Y - (0.5f * TextureDim), TextureDim, TextureDim, FColorBlack);
	}	
	VDrawTile(CursorMain, MouseLocation.X - (0.5f * CursorDim), MouseLocation.Y - (0.5f * CursorDim), CursorDim, CursorDim, FColorBlack);
	
}
 
void ASurHUD::DrawCrosshair(){	
	float TextureDim = 0.025f * ScreenRes.X;
	VDrawTile(CursorMain, (ScreenRes.X / 2.0f) - (0.5f * TextureDim), (ScreenRes.Y / 2.0f) - (0.5f * TextureDim), TextureDim, TextureDim, FColorBlack);
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
		DrawCurrentlyTracedItem();
		DrawActionBar();
		DrawPlayerStatus();		
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