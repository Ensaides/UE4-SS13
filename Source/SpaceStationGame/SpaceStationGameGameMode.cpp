// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SpaceStationGame.h"
#include "SpaceStationGameGameMode.h"
#include "SpaceStationGameHUD.h"
#include "SpaceStationGameCharacter.h"
#include "SpaceStationGamePlayerState.h"
#include "SpaceStationGameGameState.h"

#include "RoundType.h"
#include "RoundTypeTraitor.h"

#if UE_SERVER || UE_EDITOR
#include "SpaceStationGameServerState.h"
#endif

#include "SpaceStationGamePlayerController.h"

ASpaceStationGameGameMode::ASpaceStationGameGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASpaceStationGameHUD::StaticClass();

	PlayerControllerClass = ASpaceStationGamePlayerController::StaticClass();

	PlayerStateClass = ASpaceStationGamePlayerState::StaticClass();

	GameStateClass = ASpaceStationGameGameState::StaticClass();

	RoundStartDelay = 30.f;

	bDelayedStart = false;

	RoundTypeRotation.Add(ARoundTypeTraitor::StaticClass());
}

void ASpaceStationGameGameMode::BeginPlay()
{
	int32 RandomGameType = FMath::RandRange(0, RoundTypeRotation.Num() - 1);

	UWorld* const World = GetWorld();

	RoundType = World->SpawnActor<ARoundType>(RoundTypeRotation[RandomGameType], FVector(0, 0, 0), FRotator(0, 0, 0));

	if (bDelayedStart)
	{
		SetMatchState(MatchState::WaitingToStart);

		Cast<ASpaceStationGameGameState>(GetWorld()->GetGameState())->StartMatchTimer(RoundStartDelay);

		UE_LOG(SpaceStationGameLog, Warning, TEXT("Starting round timer"));

		bUseRoundStartReady = true;
	}
	else
	{
		bUseRoundStartReady = false;
	}
}

void ASpaceStationGameGameMode::StartRound()
{
	bDelayedStart = false;
}

void ASpaceStationGameGameMode::HandleMatchHasStarted()
{
	// most of this crap was copied from an engine function
	UE_LOG(SpaceStationGameLog, Log, TEXT("Starting round"));

	GameSession->HandleMatchHasStarted();

	// start human players first
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ASpaceStationGamePlayerController* PlayerController = Cast<ASpaceStationGamePlayerController>(*Iterator);
		if (bUseRoundStartReady)
		{
			if (PlayerController && (PlayerController->GetPawn() == NULL) && PlayerCanRestart(PlayerController) && PlayerController->RoundStartReady)
			{
				RestartPlayer(PlayerController);
			}
		}
		else
		{
			if (PlayerController && (PlayerController->GetPawn() == NULL) && PlayerCanRestart(PlayerController))
			{
				RestartPlayer(PlayerController);
			}
		}
	}

	// Make sure level streaming is up to date before triggering NotifyMatchStarted
	GEngine->BlockTillLevelStreamingCompleted(GetWorld());

	// First fire BeginPlay, if we haven't already in waiting to start match
	GetWorldSettings()->NotifyBeginPlay();

	// Then fire off match started
	GetWorldSettings()->NotifyMatchStarted();

	// if passed in bug info, send player to right location
	FString BugLocString = UGameplayStatics::ParseOption(OptionsString, TEXT("BugLoc"));
	FString BugRotString = UGameplayStatics::ParseOption(OptionsString, TEXT("BugRot"));
	if (!BugLocString.IsEmpty() || !BugRotString.IsEmpty())
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = *Iterator;
			if (PlayerController->CheatManager != NULL)
			{
				//`log( "BugLocString:" @ BugLocString );
				//`log( "BugRotString:" @ BugRotString );

				PlayerController->CheatManager->BugItGoString(BugLocString, BugRotString);
			}
		}
	}

	if (IsHandlingReplays() && GetGameInstance() != nullptr)
	{
		GetGameInstance()->StartRecordingReplay(GetWorld()->GetMapName(), GetWorld()->GetMapName());

	}
	//Initialize the round
	RoundType->InitializeRound();

	UE_LOG(SpaceStationGameLog, Log, TEXT("Round type is: %s"), *RoundType->RoundTypeName);
}

void ASpaceStationGameGameMode::PreLogin(const FString & Options, const FString & Address, const TSharedPtr< const FUniqueNetId > & UniqueId, FString & ErrorMessage)
{
	FString UniqueIdString = UniqueId->ToString();

	

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void ASpaceStationGameGameMode::PostLogin(APlayerController* NewPlayer)
{
#if UE_SERVER || UE_EDITOR
	if (Cast<ASpaceStationGameGameState>(GetWorld()->GetGameState()) && GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer)
	{
		Cast<ASpaceStationGamePlayerController>(NewPlayer)->LoadingState.Add("Loading MySQL player data");

		ASpaceStationGameGameState* GameState = Cast<ASpaceStationGameGameState>(GetWorld()->GetGameState());

		GameState->GetServerState()->SetUpMySQLPlayerData(NewPlayer);
	}
#endif

	Super::PostLogin(NewPlayer);
}

void ASpaceStationGameGameMode::RestartPlayer(AController* NewPlayer)
{
	if (NewPlayer == NULL || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	UE_LOG(SpaceStationGameLog, Verbose, TEXT("RestartPlayer %s"), (NewPlayer && NewPlayer->PlayerState) ? *NewPlayer->PlayerState->PlayerName : TEXT("Unknown"));

	if (NewPlayer->PlayerState && NewPlayer->PlayerState->bOnlySpectator)
	{
		//UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayer tried to restart a spectator-only player!"));
		return;
	}

	AActor* StartSpot = FindPlayerStart(NewPlayer);

	// if a start spot wasn't found,
	if (StartSpot == NULL)
	{
		// check for a previously assigned spot
		if (NewPlayer->StartSpot != NULL)
		{
			StartSpot = NewPlayer->StartSpot.Get();
		}
		else
		{
			return;
		}
	}

	// try to create a pawn to use of the default class for this player
	if (NewPlayer->GetPawn() == NULL && GetDefaultPawnClassForController(NewPlayer) != NULL)
	{
		auto Pawn = SpawnDefaultPawnFor(NewPlayer, StartSpot);

		if (Cast<ASpaceStationGameCharacter>(Pawn) && Cast<ASpaceStationGamePlayerState>(NewPlayer->PlayerState))
		{
			Cast<ASpaceStationGameCharacter>(Pawn)->OwnedPlayerID = Cast<ASpaceStationGamePlayerState>(NewPlayer->PlayerState)->UniqueId->ToString();
		}

		if (Cast<ASpaceStationGamePlayerController>(NewPlayer))
		{
			Cast<ASpaceStationGamePlayerController>(NewPlayer)->HandleRoundStart();
		}

		NewPlayer->SetPawn(Pawn);
	}

	if (NewPlayer->GetPawn() == NULL)
	{
		NewPlayer->FailedToSpawnPawn();
	}
	else
	{
		// initialize and start it up
		InitStartSpot(StartSpot, NewPlayer);

		// @todo: this was related to speedhack code, which is disabled.
		/*
		if ( NewPlayer->GetAPlayerController() )
		{
		NewPlayer->GetAPlayerController()->TimeMargin = -0.1f;
		}
		*/
		NewPlayer->Possess(NewPlayer->GetPawn());

		// If the Pawn is destroyed as part of possession we have to abort
		if (NewPlayer->GetPawn() == nullptr)
		{
			NewPlayer->FailedToSpawnPawn();
		}
		else
		{
			// set initial control rotation to player start's rotation
			NewPlayer->ClientSetRotation(NewPlayer->GetPawn()->GetActorRotation(), true);

			FRotator NewControllerRot = StartSpot->GetActorRotation();
			NewControllerRot.Roll = 0.f;
			NewPlayer->SetControlRotation(NewControllerRot);

			// Initialize the job on the new player
			auto Character =  Cast<ASpaceStationGameCharacter>(NewPlayer->GetPawn());
			Character->bInitializeJob = true;

			SetPlayerDefaults(NewPlayer->GetPawn());

			K2_OnRestartPlayer(NewPlayer);
		}
	}

#if !UE_WITH_PHYSICS
	if (NewPlayer->GetPawn() != NULL)
	{
		UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(NewPlayer->GetPawn()->GetMovementComponent());
		if (CharacterMovement)
		{
			CharacterMovement->bCheatFlying = true;
			CharacterMovement->SetMovementMode(MOVE_Flying);
		}
	}
#endif	//!UE_WITH_PHYSICS
}

void ASpaceStationGameGameMode::HandleMatchHasEnded()
{
	RoundType->EndRound();

	Super::HandleMatchHasEnded();
}