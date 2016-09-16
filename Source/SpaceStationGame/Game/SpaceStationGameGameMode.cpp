// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "SpaceStationGame.h"
#include "SpaceStationGameGameMode.h"
#include "SpaceStationGameGameState.h"
#include "SpaceStationGamePlayerController.h"
#include "SpaceStationGameHUD.h"
#include "SpaceStationGameServerState.h"
#include "SpaceStationGameCharacter.h"
#include "JobManager.h"

namespace MatchState
{
	const FName WaitingRoundStart = FName(TEXT("WaitingRoundStart"));
	const FName RoundInProgress = FName(TEXT("RoundInProgress"));
	const FName ShuttleCalled = FName(TEXT("ShuttleCalled"));
	const FName WaitingPostRound = FName(TEXT("WaitingPostRound"));
}

ASpaceStationGameGameMode::ASpaceStationGameGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	GameStateClass = ASpaceStationGameGameState::StaticClass();
	HUDClass = ASpaceStationGameHUD::StaticClass();
	PlayerControllerClass = ASpaceStationGamePlayerController::StaticClass();

	ServerStateClass = ASpaceStationGameServerState::StaticClass();

	bDelayedRoundStart = false;
	RoundStartDelay = 5.f;
}

void ASpaceStationGameGameMode::StartMatch()
{
	Super::StartMatch();

	// Set the match state to waiting for round
	SetMatchState(MatchState::WaitingRoundStart);
}

void ASpaceStationGameGameMode::HandleMatchHasStarted()
{
	GameSession->HandleMatchHasStarted();

	// Don't start players

	// Make sure level streaming is up to date before triggering NotifyMatchStarted
	GEngine->BlockTillLevelStreamingCompleted(GetWorld());

	// First fire BeginPlay, if we haven't already in waiting to start match
	GetWorldSettings()->NotifyBeginPlay();

	// Then fire off match started
	GetWorldSettings()->NotifyMatchStarted();
}

void ASpaceStationGameGameMode::HandleWaitingForRound()
{
	// If the round is delayed, start the timer
	if (bDelayedRoundStart)
	{
		GetWorldTimerManager().SetTimer(RoundStartTimer, this, &ASpaceStationGameGameMode::StartRound, RoundStartDelay, false);
	}
	else // Else start the round immediately
	{
		StartRound();
	}
}

void ASpaceStationGameGameMode::StartRound()
{
	SetMatchState(MatchState::RoundInProgress);
}

void ASpaceStationGameGameMode::HandleRoundHasStarted()
{
	// if passed in bug info, send player to right location
	const FString BugLocString = UGameplayStatics::ParseOption(OptionsString, TEXT("BugLoc"));
	const FString BugRotString = UGameplayStatics::ParseOption(OptionsString, TEXT("BugRot"));
	if (!BugLocString.IsEmpty() || !BugRotString.IsEmpty())
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = *Iterator;
			if (PlayerController->CheatManager != NULL)
			{
				PlayerController->CheatManager->BugItGoString(BugLocString, BugRotString);
			}
		}
	}

	if (IsHandlingReplays() && GetGameInstance() != nullptr)
	{
		GetGameInstance()->StartRecordingReplay(GetWorld()->GetMapName(), GetWorld()->GetMapName());
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = *Iterator;
		if ((PlayerController->GetPawn() == NULL) && PlayerCanRestart(PlayerController))
		{
			RestartPlayer(PlayerController);

			Cast<ASpaceStationGamePlayerController>(PlayerController)->AddChatMessage("Welcome to the station crew, enjoy your stay!", MESSAGE_NOTIFICATION_COLOR);
		}
	}
}

void ASpaceStationGameGameMode::CallShuttle()
{
	SetMatchState(MatchState::ShuttleCalled);
}

void ASpaceStationGameGameMode::RecallShuttle()
{
	SetMatchState(MatchState::RoundInProgress);
}

void ASpaceStationGameGameMode::HandleShuttleCalled()
{

}

void ASpaceStationGameGameMode::EndRound()
{
	SetMatchState(MatchState::WaitingPostRound);
}

void ASpaceStationGameGameMode::HandleRoundHasEnded()
{

}

bool ASpaceStationGameGameMode::IsMatchInProgress() const
{
	if (GetMatchState() == MatchState::RoundInProgress)
	{
		return true;
	}
	if (MatchState == MatchState::ShuttleCalled)
	{
		return true;
	}

	return Super::IsMatchInProgress();
}

void ASpaceStationGameGameMode::OnMatchStateSet()
{
	if (MatchState == MatchState::WaitingRoundStart)
	{
		HandleWaitingForRound();
	}
	if (MatchState == MatchState::RoundInProgress)
	{
		HandleRoundHasStarted();
	}
	if (MatchState == MatchState::ShuttleCalled)
	{
		HandleShuttleCalled();
	}
	if (MatchState == MatchState::WaitingPostRound)
	{
		HandleRoundHasEnded();
	}

	Super::OnMatchStateSet();
}

void ASpaceStationGameGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}

void ASpaceStationGameGameMode::SetPlayerDefaults(APawn* PlayerPawn)
{
	Super::SetPlayerDefaults(PlayerPawn);

	// Set up the job for the player
	auto World = GetWorld();
	if (World)
	{
		if (Cast<ASpaceStationGameGameState>(GameState))
		{
			auto ServerState = Cast<ASpaceStationGameGameState>(GameState)->GetServerState();

			ServerState->GetJobManager()->SetupJob(PlayerPawn);
		}
	}
}