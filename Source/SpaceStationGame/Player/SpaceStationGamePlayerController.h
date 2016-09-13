// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "ChatStructs.h"
#include "SpaceStationGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPACESTATIONGAME_API ASpaceStationGamePlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

	// Chat
	// Array of the chat for referential purposes, only on the server
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		TArray<FClientChatMessageStruct> ChatMessages;
	
	// Called by the server to add a new chat message
	void AddChatMessage(FString Msg, bool Radio);

	// Server calls this after AddChatMessageIsCalled
	UFUNCTION(Client, Reliable, WithValidation)
		void Client_AddChatMessage(const FString& Msg, bool Radio);

	//UFUNCTION(exec)
	//	void Say(const FString& InputString);

public:
	// Job
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Job)
		TSubclassOf<class UJob> StartingJob;

public:
	virtual void Tick(float DeltaSeconds) override;

//#ifdef UE_BUILD_DEBUG
public:
	// Debug members
	void SetAtmosphericsFPS(float NewFPS) { AtmosphericsFPS = NewFPS; };

	UPROPERTY(Replicated)
		float AtmosphericsFPS;
//#endif
};
