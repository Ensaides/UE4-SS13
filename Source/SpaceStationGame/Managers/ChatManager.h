// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Manager.h"
#include "ChatStructs.h"
#include "ChatManager.generated.h"

class ASpaceStationGamePlayerController;

/**
 * 
 */
UCLASS()
class SPACESTATIONGAME_API AChatManager : public AManager
{
	GENERATED_UCLASS_BODY()
	
	// Array of chat messages on the server
	TArray<FChatMessageStruct> ChatMessages;

	void ReceieveChatMessage(ASpaceStationGamePlayerController* Controller, FString Msg);
	
	// Send the messages to the client player controllers from a player
	void SendNewChatMessage(const FString Msg, const FString PlayerName, FVector Location);

	// How far away a person can hear someone talking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		float fMaxChatDist;
};
