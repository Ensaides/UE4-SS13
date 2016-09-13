// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "SpaceStationGameGameState.h"

#if UE_SERVER || UE_EDITOR
#include "SpaceStationGameServerState.h"
#endif

#include "SpaceStationGameGameMode.h"
#include "InstanceManager.h"

ASpaceStationGameGameState::ASpaceStationGameGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceManagerClass = AInstanceManager::StaticClass();
}

void ASpaceStationGameGameState::BeginPlay()
{
	UWorld* const World = GetWorld();

#if UE_SERVER || UE_EDITOR
	if (GetNetMode() != ENetMode::NM_Client)
	{
		auto GameMode = Cast<ASpaceStationGameGameMode>(World->GetAuthGameMode());

		// Spawn the server state
		ServerState = World->SpawnActor<ASpaceStationGameServerState>(GameMode->ServerStateClass);
	}
#endif

	// Managers
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.bNoFail = true;

	const FTransform LocRot;

	if (GetNetMode() != ENetMode::NM_DedicatedServer) InstanceManager = World->SpawnActor<AInstanceManager>(InstanceManagerClass, LocRot, SpawnParams);
	
	// The order we initialize these doesn't really matter
	if (GetNetMode() != ENetMode::NM_DedicatedServer) InstanceManager->Initialize();

	// Call the BP initializers
	BP_InitializeManagers();

	Super::BeginPlay();
}
