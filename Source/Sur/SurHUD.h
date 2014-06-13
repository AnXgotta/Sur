

#pragma once

#include "GameFramework/HUD.h"
#include "SurHUD.generated.h"

USTRUCT()
struct FSurButtonStruct {
	GENERATED_USTRUCT_BODY()

	//Vars
	int32 type;
	FString	toolTip;
	float minX;
	float maxX;
	float minY;
	float maxY;

	//~

	//default properties

	FSurButtonStruct()	{
		type = -1;
		toolTip = "";
		minX = 0;
		maxX = 0;
		minY = 0;
		maxY = 0;
	}
};


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

	/* Draw Hud? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Options)
		bool DontDrawHUD;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SurHUD)
	UFont* UIFont;

	/** Font Scaling Used By HUD Code */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SurHUD)
		float DefaultFontScale;

	/** HUD Scaling */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SurHUD)
		float GlobalHUDMult;

	/** Button */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = T2D)
		UTexture2D* ButtonBackground;

	/** Cursor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = T2D)
		UTexture2D* CursorMain;

	/** Hovering */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = T2D)
		UTexture2D* CursorHoveringButton;



	/** Events */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
		UMaterialInterface* MaterialBackground;

	//Cursor
public:
	FVector2D MouseLocation;
	void DrawHUD_DrawCursor();

	//Buttons
public:
	TArray<FSurButtonStruct> ButtonsMain;
	TArray<FSurButtonStruct> ButtonsConfirm;

	//Cursor In buttons
	void DrawHUD_CheckCursorInButtons();
	void CheckCursorInButtonsMain();
	void CheckCursorInButtonsConfirm();

	const FSurButtonStruct* CurCheckButton;
	int32 CheckCursorInButton(const TArray<FSurButtonStruct>& ButtonArray);
	int32 ClickedButtonType;
	//States
	bool ConfirmDialogOpen;
	bool InMainMenu;

	int32 		ActiveButton_Type;
	FString 	ActiveButton_Tip;
	bool CursorHoveringInButton;



	//Colors
public:
	const FLinearColor * ColorPtr;

	//Colors
	static const FColor		FColorBlack;
	static const FColor		FColorRed;
	static const FColor		FColorYellow;
	static const FColor		FColorBlue;
	static const FColor		FColor_White;

	static const FLinearColor LC_Black;
	static const FLinearColor LC_Pink;
	static const FLinearColor LC_Red;
	static const FLinearColor LC_Yellow;
	//FString
public:

	//`Titles
	static const FString S_Title_Main;
	static const FString S_Title_Confirm;
	//`Button Text
	static const FString S_Button_Restart;
	static const FString S_Button_Exit;

	// Utility 

	//Stop Camera From Moving With Mouse
	FORCEINLINE void SetCursorMoveOnly(bool CursorOnly)
	{
		if (!ThePC) return;
		//
		ThePC->SetIgnoreLookInput(CursorOnly);

	}

	//DrawLine
	FORCEINLINE void DrawSurLine
		(
		const FVector2D& Start,
		const FVector2D& End,
		const FLinearColor& TheColor,
		const float& Thick
		)
	{
		if (!Canvas) return;
		//

		FCanvasLineItem NewLine(Start, End);
		NewLine.SetColor(TheColor);
		NewLine.LineThickness = Thick;
		Canvas->DrawItem(NewLine);
	}

	//~

	FORCEINLINE void DrawSurRect(
		float X, float Y,
		float Width, float Height,
		const FLinearColor& Color
		)
	{
		if (!Canvas) return;
		//

		FCanvasTileItem RectItem(
			FVector2D(X, Y),
			FVector2D(Width, Height),
			Color
			);

		RectItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(RectItem);
	}

	//~

	//DrawText
	FORCEINLINE void DrawSurText(
		UFont*	TheFont,
		const FString& TheStr,
		const float& X, const float& Y,
		const FLinearColor& TheColor,
		const float& TheScale,
		bool DrawOutline = false,
		const FLinearColor OutlineColor = FLinearColor(0, 0, 0, 1)
		) {
		if (!Canvas) return;
		//

		//Text and Font
		FCanvasTextItem NewText(
			FVector2D(X, Y),
			FText::FromString(TheStr),
			TheFont,
			TheColor
			);

		//Text Scale
		NewText.Scale.Set(TheScale, TheScale);

		//Outline gets its alpha from the main color
		NewText.bOutlined = true;
		NewText.OutlineColor = OutlineColor;
		NewText.OutlineColor.A = TheColor.A * 2;

		//Draw
		Canvas->DrawItem(NewText);
	}

	//~
	//Draw Full Size Tile
	FORCEINLINE void DrawFullSizeTile(UTexture2D* tex, float x, float y, const FColor& Color)
	{
		if (!Canvas) return;
		if (!tex) return;
		//~~

		Canvas->SetDrawColor(Color);

		//Draw
		Canvas->DrawTile(
			tex, x, y, //z pos
			tex->GetSurfaceWidth(), //screen width
			tex->GetSurfaceHeight(),  //screen height
			0, //texture start width
			0, //texture start height
			tex->GetSurfaceWidth(), //texture width from start
			tex->GetSurfaceHeight(), //texture height from start
			BLEND_Translucent
			);
	}

	//~

	FORCEINLINE void VDrawTile(UTexture2D* tex, float x, float y, float screenX, float screenY, const FColor& TheColor)
	{
		if (!Canvas) return;
		if (!tex) return;
		//~

		Canvas->SetDrawColor(TheColor);

		//Draw
		Canvas->DrawTile(
			tex, x, y, //z pos
			screenX, //screen width
			screenY,  //screen height
			0, //texture start width
			0, //texture start height
			tex->GetSurfaceWidth(), //texture width from start
			tex->GetSurfaceHeight(), //texture height from start
			BLEND_Translucent
			);
	}

	//~

	//Draw
public:
	void DrawHUD_DrawDialogs();

	//Menus
	void DrawMainMenu();
	void DrawConfirm();

	//Buttons
	void DrawMainMenuButtons();
	void DrawConfirmButtons();
public:
	void DrawToolTip();

	//Core
public:
	APlayerController* ThePC;
	void PlayerInputChecks();
protected:
	//Draw HUD
	void DrawHUD_Reset();
	virtual void DrawHUD() OVERRIDE;

	/** after all game elements are created */
	virtual void PostInitializeComponents() OVERRIDE;




};
