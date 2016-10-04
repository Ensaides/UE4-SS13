// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "TilesetManager.h"
#include "Tileset.h"

ATileset::ATileset(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAlreadyConstructed = false;
}

void ATileset::OnConstruction(const FTransform& Transform)
{
	if (IsGameWorld())
	{
		if (bAlreadyConstructed)
		{
			ATilesetManager::RemoveTile(TileIndex, this, true);
		}

		// If we succeeded in adding the tile, bAlreadyConstructed = true
		bAlreadyConstructed = ATilesetManager::AddTile(this, TileIndex);
	}

	Super::OnConstruction(Transform);
}

AActor* ATileset::SpawnActorInEditor(UClass* ActorClass, FTransform Transform)
{
	if (GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bAllowDuringConstructionScript = true;
		SpawnParams.bNoFail = true;

		return GetWorld()->SpawnActor(ActorClass, &Transform, SpawnParams);
	}

	return NULL;
}

bool ATileset::IsGameWorld()
{
	auto World = GetWorld();
	if (!World)
	{
		return false;
	}

	// World type 1 is the game world, type 2 is the editor world
	if (World->WorldType == 1 || World->WorldType == 2)
	{
		return true;
	}

	return false;
}

void ATileset::CleanupTile()
{
	if (bAlreadyConstructed)
	{
		ATilesetManager::RemoveTile(TileIndex);
	}
}

void ATileset::BeginDestroy()
{
	UE_LOG(LogTemp, Log, TEXT("*** BeginDestroy() entered. ***"));

	CleanupTile();

	Super::BeginDestroy();
}