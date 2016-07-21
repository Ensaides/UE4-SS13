// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"

#include "SpaceStationGameGameState.h"
#include "InstanceManager.h"

#include "InstancedItem.h"


AInstancedItem::AInstancedItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AInstancedItem::BeginPlay()
{
	if (GetNetMode() != ENetMode::NM_DedicatedServer)
	{
		auto const World = GetWorld();

		auto GameState = World->GetGameState<ASpaceStationGameGameState>();

		if (GameState)
		{
			auto InstanceManager = Cast<AInstanceManager>(GameState->GetInstanceManager());

			InstanceManager->CreateNewInstanceContainer(this);

			InstanceManager->CreateNewInstance(this);

			Mesh->UnregisterComponent();
		}
	}
}