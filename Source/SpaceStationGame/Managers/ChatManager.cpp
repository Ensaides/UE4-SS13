// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "SpaceStationGamePlayerController.h"
#include "SpaceStationGameCharacter.h"
#include "ChatManager.h"

AChatManager::AChatManager(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	fMaxChatDist = 500.f;
}

void AChatManager::ReceieveChatMessage(ASpaceStationGamePlayerController* Controller, FString Msg)
{
	auto Character = Cast<ASpaceStationGameCharacter>(Controller->GetPawn());

	if (Character)
	{
		SendNewChatMessage(Msg, Character->NickName, Character->GetActorLocation());
	}
}

void AChatManager::SendNewChatMessage(const FString Msg, const FString PlayerName, FVector Location)
{
	for (TActorIterator<ASpaceStationGamePlayerController> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->GetPawn() && (ActorItr->GetPawn()->GetActorLocation() - Location).Size() < fMaxChatDist)
		{
			ActorItr->AddChatMessage(PlayerName + FString(" says, \"") + Msg + FString("\""), MESSAGE_DEFAULT_COLOR);
		}
		else if (ActorItr->GetSpectatorPawn())
		{
			ActorItr->AddChatMessage(PlayerName + FString(" says, \"") + Msg + FString("\""), MESSAGE_DEFAULT_COLOR);
		}
	}
}
