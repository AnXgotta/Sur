

#pragma once

#include "GameFramework/HUD.h"
#include "SurInventorySlot.h"
#include "SurTypes.h"
#include "SurHUD.generated.h"




UCLASS()
class ASurHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	UMaterialInterface* MaterialBackground;

	virtual void DrawHUD() OVERRIDE;
	virtual void PostInitializeComponents() OVERRIDE;

	APlayerController* ThePC;

	//  DRAW STATES  ######################################################################

	UPROPERTY()
	bool bDrawInventory;

	UPROPERTY()
		bool bDrawChat;

	UPROPERTY()
		bool bDrawHUD;


	//  INTERACTION STATES  ###############################################################

	UPROPERTY()
		bool bIsHoldingLMB;


	//  CRAFTING  ######################################################################

	UPROPERTY(EditDefaultsOnly, Category=CraftableList)
		TArray<FCraftableItem> CraftableItemList;

	UPROPERTY()
		TArray<FCraftableItem> CurrentlyCraftableList;

		TMap<FName, int32> ItemizedInventoryMap;

	UFUNCTION()
		void DrawCurrentlyCraftableList();

	UFUNCTION()
		void UpdateCurrentlyCraftableList();

	UFUNCTION()
		void CraftItem(FCraftableItem CraftableItem);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerCraftItem(FCraftableItem CraftableItem);


	//  TEXT  ############################################################################

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SurHUD)
	UFont* UIFont;

	/** Font Scaling Used By HUD Code */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SurHUD)
		float DefaultFontScale;


	//  HUD SCALE

public:

	FVector2D ScreenRes;


	UFUNCTION(BlueprintCallable, Category = ScreenResolution)
		void SetScreenResolution();


	/** HUD Scaling */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SurHUD)
		float GlobalHUDMult;


	//  BUTTONS  ##########################################################

	TArray<FSurButtonStruct> ButtonsActionBar;
	TArray<FSurButtonStruct> ButtonsInventory;
	TArray<FSurButtonStruct> ButtonsCrafting;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = T2D)
		UTexture2D* InventorySlotBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = T2D)
		UTexture2D* ActiveInventorySlotBackground;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = T2D)
		UTexture2D* ButtonBackground;


	

	//  CURSOR  #############################################################

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = T2D)
		UTexture2D* CursorMain;

	

	FVector2D MouseLocation;

	void DrawCursor();


	//  INTERACTION  #########################################################

	const FSurButtonStruct* CurCheckButton;
	const FSurButtonStruct* CurDraggedButton;
	USurInventorySlot* CurInventorySlot;
	bool bIsDraggingButton;
	int32 CheckCursorInButton(const TArray<FSurButtonStruct>& ButtonArray);
	int32 ClickedButtonType;

	int32 ActiveButton_Type;
	FString ActiveButton_Tip;

	// INPUT  #####################################################################

	void PlayerInputChecks();

	void ResetStateInfo();
	

	//  STATIC COLORS  ################################################################

public:
	const FLinearColor * ColorPtr;

	static const FColor		FColorBlack;
	static const FColor		FColorRed;
	static const FColor		FColorYellow;
	static const FColor		FColorBlue;
	static const FColor		FColor_White;

	static const FLinearColor LC_Black;
	static const FLinearColor LC_Pink;
	static const FLinearColor LC_Red;
	static const FLinearColor LC_Yellow;
	static const FLinearColor LC_Transparent;

	//  DRAWING  ##################################################################

	void DrawHUD_DrawDialogs();

	void DrawActionBar();

	void DrawInventory();

	void DrawCrosshair();

	void DrawToolTip();

	void DrawPlayerStatus();
	
	void DrawCurrentlyTracedItem(); 




	// UTILITY  ######################################################################

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
		//NewText.bOutlined = true;
		//NewText.OutlineColor = OutlineColor;
		//NewText.OutlineColor.A = TheColor.A * 2;

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

		//Canvas->SetDrawColor(TheColor);

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




};
