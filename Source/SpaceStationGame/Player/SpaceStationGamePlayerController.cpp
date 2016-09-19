// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "SpaceStationGamePlayerController.h"
#include "Assistant.h"
#include "SpaceStationGameHud.h"
#include "SpaceStationGameGameState.h"
#include "SpaceStationGameServerState.h"
#include "ChatManager.h"
#include "UnrealNetwork.h"

ASpaceStationGamePlayerController::ASpaceStationGamePlayerController(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	StartingJob = UAssistant::StaticClass();
	bRoundStartReady = true;
}

void ASpaceStationGamePlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Job
	DOREPLIFETIME_CONDITION(ASpaceStationGamePlayerController, StartingJob, COND_OwnerOnly);

	// Round
	DOREPLIFETIME(ASpaceStationGamePlayerController, bRoundStartReady);

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

void ASpaceStationGamePlayerController::AddChatMessage(FString Msg, FLinearColor Color)
{
	FClientChatMessageStruct NewMessage;

	NewMessage.ChatMsg = Msg;
	NewMessage.MessageColor = Color;

	ChatMessages.Add(NewMessage);

	Client_AddChatMessage(Msg, Color);
}

bool ASpaceStationGamePlayerController::Client_AddChatMessage_Validate(const FString& Msg, FLinearColor Color)
{
	return true;
}

void ASpaceStationGamePlayerController::Client_AddChatMessage_Implementation(const FString& Msg, FLinearColor Color)
{
	FClientChatMessageStruct NewMessage;

	NewMessage.ChatMsg = Msg;
	NewMessage.MessageColor = Color;

	if (Cast<ASpaceStationGameHUD>(GetHUD()))
		Cast<ASpaceStationGameHUD>(GetHUD())->BP_AddChatMessage(NewMessage);
}

void ASpaceStationGamePlayerController::Say(const FString& InputString)
{
	Server_Say(InputString);
}

bool ASpaceStationGamePlayerController::Server_Say_Validate(const FString& InputString)
{
	return true;
}

void ASpaceStationGamePlayerController::Server_Say_Implementation(const FString& InputString)
{
	auto GameState = Cast<ASpaceStationGameGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		auto ServerState = GameState->GetServerState();

		ServerState->GetChatManager()->ReceieveChatMessage(this, InputString);
	}
}

bool ASpaceStationGamePlayerController::SetRoundStartReady_Validate(bool bNewReady)
{
	return true;
}

void ASpaceStationGamePlayerController::SetRoundStartReady_Implementation(bool bNewReady)
{
	bRoundStartReady = bNewReady;
}