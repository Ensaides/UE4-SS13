// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Manager.h"
#include "ChatStructs.h"
#include "ChatManager.generated.h"


/**
 * 
 */
UCLASS()
class SPACESTATIONGAME_API AChatManager : public AManager
{
	GENERATED_BODY()
	
	// Array of chat messages on the server
	TArray<FChatMessageStruct> ChatMessages;
	
	// Send the messages to the client player controller
	void SendNewChatMessage(const FString Msg, const FString PlayerName, FVector Location);

public:
	// Configs

	// How far away a person can hear someone talking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		float fMaxChatDist;
};
