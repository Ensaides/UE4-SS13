// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "SpaceStationGameGameMode.generated.h"

// Match states for the space station game mode
namespace MatchState
{
	extern const FName WaitingRoundStart;	// Players are waiting for the round to start
	extern const FName RoundInProgress;		// Round in progress, players spawned
	extern const FName ShuttleCalled;		// Shuttle has been called, round about to end
	extern const FName WaitingPostRound;	// Round ended, players waiting post round
}

UCLASS()
class ASpaceStationGameGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes, meta = (DisplayName = "Server State Class"))
		TSubclassOf<class ASpaceStationGameServerState> ServerStateClass;

	// Sets up the player's job
	virtual void SetPlayerDefaults(APawn* PlayerPawn);

public:

	/** Returns true if the round is in progress **/
	UFUNCTION(BlueprintCallable, Category = "Game")
		virtual bool IsMatchInProgress() const override;

	/** Transition from WaitingToStart to InProgress. You can call this manually, will also get called if ReadyToStartMatch returns true */
	UFUNCTION(BlueprintCallable, Category = "Game")
		virtual void StartMatch() override;

	// Rounds
	// Called by the round actor after waiting for the round to start
	UFUNCTION(BlueprintCallable, Category = "Round")
		virtual void StartRound();

	// Called by the round actor after the round ends
	UFUNCTION(BlueprintCallable, Category = "Round")
		virtual void CallShuttle();

	// Used to recall the shuttle
	UFUNCTION(BlueprintCallable, Category = "Round")
		virtual void RecallShuttle();

	// Called by the round actor after the shuttle docks
	UFUNCTION(BlueprintCallable, Category = "Round")
		virtual void EndRound();

public:
	/** Whether the round should start immediately or if there should be a round start timer */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Round")
		uint32 bDelayedRoundStart : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Round")
		float RoundStartDelay;

	virtual void RestartPlayer(class AController* NewPlayer) override;

	FTimerHandle RoundStartTimer;


protected:
	virtual void OnMatchStateSet() override;

	/** Called when the state transitions to InProgress */
	virtual void HandleMatchHasStarted() override;

	// Called after the match has started
	virtual void HandleWaitingForRound();

	// Called after round has started
	virtual void HandleRoundHasStarted();

	virtual void HandleShuttleCalled();

	virtual void HandleRoundHasEnded();
};



