// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "SpaceStationGame.h"
#include "SpaceStationGameHUD.h"
#include "Engine/Canvas.h"
#include "TextureResource.h"
#include "UMG.h"
#include "CanvasItem.h"

ASpaceStationGameHUD::ASpaceStationGameHUD(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set the crosshair texture
	//static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshiarTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	//CrosshairTex = CrosshiarTexObj.Object;
}


void ASpaceStationGameHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ASpaceStationGameHUD::BeginPlay()
{
	FText WindowTitle = FText::FromString("Chat Window");

	const TSharedPtr<SWindow> Window = SNew(SWindow)
		.Title(WindowTitle)
		.ClientSize(FVector2D(800, 600))
		.UseOSWindowBorder(true);

	/*
	.ClientSize(FVector2D(ResX, ResY))
	.Title(WindowTitle)
	.AutoCenter(AutoCenterType)
	.ScreenPosition(FVector2D(WinX, WinY))
	.MaxWidth(MaxWindowWidth)
	.MaxHeight(MaxWindowHeight)
	.FocusWhenFirstShown(true)
	.SaneWindowPlacement(AutoCenterType == EAutoCenter::None)
	.UseOSWindowBorder(true);*/

	ChatWindow = FSlateApplication::Get().AddWindow(Window.ToSharedRef());

	BP_SetupChatWindow();

	OpenRoundStartMenu();
}

void ASpaceStationGameHUD::BP_SetChatWindowContent(UWidget* InContent)
{
	ChatWindow->SetContent(InContent->TakeWidget());
}

void ASpaceStationGameHUD::OpenRoundStartMenu()
{
	//GetOwningPlayerController()->bShowMouseCursor = true;

	BP_OpenRoundStartMenu();
}

void ASpaceStationGameHUD::BeginDestroy()
{
	if (ChatWindow.IsValid())
	{
		ChatWindow->RequestDestroyWindow();
	}

	Super::BeginDestroy();
}

