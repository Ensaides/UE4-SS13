// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "ChatMessageStruct.h"
#include "InstancedItemContainer.h"
#include "Reagents.h"
#include "SpaceStationGameGameState.generated.h"

/**
*
*/
UCLASS()
class ASpaceStationGameGameState : public AGameState
{
	GENERATED_BODY()

	TArray<FChatMessageStruct*> ChatMessages;

	UFUNCTION(Client, Reliable, WithValidation)
		void SendNewChatMessage(const FString& Msg, const FString& PlayerName, FVector Location);

	bool SendNewChatMessage_Validate(const FString& Msg, const FString& PlayerName, FVector Location) { return true; };

	void SendNewChatMessage_Implementation(const FString& Msg, const FString& PlayerName, FVector Location);

	FTimerHandle RoundStartTimerHandle;

	//Job Stuff

	UPROPERTY()
	class UJobManagerObject* JobManagerObject;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Jobs)
		void SetUpJobs();

	TSubclassOf<class UJobObject> GetJob(uint8 Job);

#if UE_SERVER || UE_EDITOR
private:
	UPROPERTY()
		class ASpaceStationGameServerState* ServerState;

public:
	ASpaceStationGameServerState* GetServerState() { return ServerState; };

#endif

public:
	ASpaceStationGameGameState(const FObjectInitializer& ObjectInitializer);

	void StartMatchTimer(float TimerLength);

	void StartRound();

	UPROPERTY(Replicated)
		TArray<AInstancedItemContainer*> InstancedItemContainers;

	UPROPERTY(Replicated)
		bool bDelayedStart;

	UFUNCTION(BlueprintCallable, Category = Round)
	bool GetDelayedStart() { return bDelayedStart; };

	UPROPERTY(EditAnywhere, Category = Item)
		TArray<TSubclassOf<class AInstancedItemContainer>> InstancedItemContainerClasses;

	virtual void BeginPlay() override;

	AInstancedItemContainer* GetContainerFromClass(UClass* InputClass);


	void AddChatMessage(const FString& Msg, FVector PlayerLocation, const FString& PlayerName);

	UFUNCTION(BlueprintCallable, Category = State, meta = (DisplayName = "Get Match State"))
		FName BP_GetMatchState() { return GetMatchState(); };
};