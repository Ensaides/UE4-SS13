// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "ChatMessageStruct.h"
#include "InstancedItemContainer.h"
#include "Reagents.h"
#include "SpaceStationGameGameState.generated.h"

class UJobManagerObject;
class UJobObject;

/**
*
*/
UCLASS()
class ASpaceStationGameGameState : public AGameState
{
	GENERATED_BODY()

	TArray<FChatMessageStruct*> ChatMessages;

	// Send the messages to the client
	UFUNCTION(Client, Reliable, WithValidation)
		void SendNewChatMessage(const FString& Msg, const FString& PlayerName, FVector Location);

	bool SendNewChatMessage_Validate(const FString& Msg, const FString& PlayerName, FVector Location) { return true; };

	void SendNewChatMessage_Implementation(const FString& Msg, const FString& PlayerName, FVector Location);

	//Job Stuff
public:
	UPROPERTY(BlueprintReadOnly, Category = Jobs)
	UJobManagerObject* JobManagerObject;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Jobs)
		void SetUpJobs();

	UFUNCTION(BlueprintCallable, Category = Jobs)
	TSubclassOf<UJobObject> GetJob(uint8 Job);

	UFUNCTION(BlueprintCallable, Category = Jobs)
	TArray<TSubclassOf<UJobObject>> GetJobArray();

	UFUNCTION(BlueprintCallable, Category = Jobs)
	TArray<FString> GetJobNames();

#if UE_SERVER || UE_EDITOR
private:
	UPROPERTY()
		class ASpaceStationGameServerState* ServerState;

public:
	ASpaceStationGameServerState* GetServerState() { return ServerState; };

#endif

public:
	ASpaceStationGameGameState(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Replicated)
		TArray<AInstancedItemContainer*> InstancedItemContainers;

	// Round stuff

	UPROPERTY(Replicated)
		bool bDelayedStart;

	UFUNCTION(BlueprintCallable, Category = Round)
	bool GetDelayedStart() { return bDelayedStart; };

	void StartMatchTimer(float TimerLength);

	void StartRound();

private:
	FTimerHandle RoundStartTimerHandle;


public:
	UPROPERTY(EditAnywhere, Category = Item)
		TArray<TSubclassOf<class AInstancedItemContainer>> InstancedItemContainerClasses;

	virtual void BeginPlay() override;

	AInstancedItemContainer* GetContainerFromClass(UClass* InputClass);

	void AddChatMessage(const FString& Msg, FVector PlayerLocation, const FString& PlayerName);

	UFUNCTION(BlueprintCallable, Category = State, meta = (DisplayName = "Get Match State"))
		FName BP_GetMatchState() { return GetMatchState(); };
};