// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "SpaceStationGameServerState.h"

#include "AtmosphericsManager.h"
#include "ChatManager.h"
#include "JobManager.h"


// Sets default values
ASpaceStationGameServerState::ASpaceStationGameServerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	AtmosphericsManagerClass = AAtmosphericsManager::StaticClass();
	ChatManagerClass = AChatManager::StaticClass();
	JobManagerClass = AJobManager::StaticClass();
}

void ASpaceStationGameServerState::BeginPlay()
{
	UWorld* const World = GetWorld();

	// Managers
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.bNoFail = true;

	const FTransform LocRot;

	AtmosphericsManager = World->SpawnActor<AAtmosphericsManager>(AtmosphericsManagerClass, LocRot, SpawnParams);
	ChatManager = World->SpawnActor<AChatManager>(ChatManagerClass, LocRot, SpawnParams);
	JobManager = World->SpawnActor<AJobManager>(JobManagerClass, LocRot, SpawnParams);

	// The order we initialize these doesn't really matter
	AtmosphericsManager->Initialize();
	ChatManager->Initialize();
	JobManager->Initialize();

	// Call the BP initializers
	BP_InitializeManagers();

	Super::BeginPlay();
}

void ASpaceStationGameServerState::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

