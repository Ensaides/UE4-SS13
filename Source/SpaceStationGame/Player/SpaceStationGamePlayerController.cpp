// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "SpaceStationGamePlayerController.h"
#include "Assistant.h"
#include "SpaceStationGameHud.h"
#include "UnrealNetwork.h"

ASpaceStationGamePlayerController::ASpaceStationGamePlayerController(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	StartingJob = UAssistant::StaticClass();
}

void ASpaceStationGamePlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Job
	DOREPLIFETIME_CONDITION(ASpaceStationGamePlayerController, StartingJob, COND_OwnerOnly);

//#ifdef UE_BUILD_DEBUG
	DOREPLIFETIME_CONDITION(ASpaceStationGamePlayerController, AtmosphericsFPS, COND_OwnerOnly);
//#endif
}

void ASpaceStationGamePlayerController::Tick(float DeltaSeconds)
{
#ifdef UE_BUILD_DEBUG
	if (GetNetMode() == ENetMode::NM_Client)
	{
		FString FPS = "Atmos FPS:" + FString::SanitizeFloat(AtmosphericsFPS);

		GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Yellow, FPS);
	}
#endif
}

void ASpaceStationGamePlayerController::AddChatMessage(FString Msg, bool Radio)
{
	FClientChatMessageStruct NewMessage;

	if (Radio)
	{
		NewMessage.MessageColor = FColor::Red;
	}
	else
	{
		NewMessage.MessageColor = FColor::Black;
	}

	NewMessage.ChatMsg = Msg;
	NewMessage.MessageColor = FColor::Black;

	ChatMessages.Add(NewMessage);

	Client_AddChatMessage(Msg, Radio);
}

bool ASpaceStationGamePlayerController::Client_AddChatMessage_Validate(const FString& Msg, bool Radio)
{
	return true;
}

void ASpaceStationGamePlayerController::Client_AddChatMessage_Implementation(const FString& Msg, bool Radio)
{
	FClientChatMessageStruct NewMessage;

	if (Radio)
	{
		NewMessage.MessageColor = FColor::Red;
	}
	else
	{
		NewMessage.MessageColor = FColor::Black;
	}

	NewMessage.ChatMsg = Msg;
	NewMessage.MessageColor = FColor::Black;

	if (Cast<ASpaceStationGameHUD>(GetHUD()))
		Cast<ASpaceStationGameHUD>(GetHUD())->BP_AddChatMessage(NewMessage);
}
