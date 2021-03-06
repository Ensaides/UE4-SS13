// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "ChatStructs.h"
#include "Objective.h"
#include "SpaceStationGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPACESTATIONGAME_API ASpaceStationGamePlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

	// The objectives that the player has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objectives")
		TArray<UObjective*> Objectives;

	// Chat
	// Array of the chat for referential purposes, only on the server
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		TArray<FClientChatMessageStruct> ChatMessages;
	
	// Called by the server to add a new chat message
	void AddChatMessage(FString Msg, FLinearColor Color);

	// Server calls this after AddChatMessageIsCalled
	UFUNCTION(Client, Reliable, WithValidation)
		void Client_AddChatMessage(const FString& Msg, FLinearColor Color);

	UFUNCTION(exec)
		void Say(const FString& InputString);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Say(const FString& InputString);

public:
	// Job
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Job")
		TSubclassOf<class UJob> StartingJob;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Round")
		bool bRoundStartReady;

	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation, Category = "Round")
		void SetRoundStartReady(bool bNewReady);
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
