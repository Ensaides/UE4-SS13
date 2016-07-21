// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "SpaceStationGamePlayerController.h"
#include "ChatManager.h"

void AChatManager::SendNewChatMessage(const FString Msg, const FString PlayerName, FVector Location)
{
	for (TActorIterator<ASpaceStationGamePlayerController> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->GetPawn() && (ActorItr->GetPawn()->GetActorLocation() - Location).Size() < fMaxChatDist)
		{
			ActorItr->AddChatMessage(PlayerName + FString(" says, \"") + Msg + FString("\""), false);
		}
		else if (ActorItr->GetSpectatorPawn())
		{
			ActorItr->AddChatMessage(PlayerName + FString(" says, \"") + Msg + FString("\""), false);
		}
	}
}
