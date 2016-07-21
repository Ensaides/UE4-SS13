// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "ChatStructs.h"
#include "SpaceStationGameHUD.generated.h"

UCLASS()
class ASpaceStationGameHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (ToolTip = "Implement this to add chat messages to the hud", DisplayName = "Add Chat Message"), Category = Chat)
		void BP_AddChatMessage(FClientChatMessageStruct NewMessage);

	UFUNCTION(BlueprintImplementableEvent, meta = (ToolTip = "Implement this to set up the chat window", DisplayName = "Setup Chat Window"), Category = Chat)
		void BP_SetupChatWindow();

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Set the content of the chat window", DisplayName = "Set Chat Window Content"), Category = Chat)
		void BP_SetChatWindowContent(class UWidget* InContent);

	void OpenRoundStartMenu();

	UFUNCTION(BlueprintImplementableEvent, meta = (ToolTip = "Implement this to open the round start menu", DisplayName = "Open Round Start Menu"), Category = Chat)
		void BP_OpenRoundStartMenu();
public:
	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	TSharedPtr<class SWindow> ChatWindow;

};

