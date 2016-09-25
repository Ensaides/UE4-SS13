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
	if (bAlreadyConstructed && LastTileLocation != Transform.GetTranslation())
	{
		ATilesetManager::RemoveTileAtLocation(LastTileLocation);
	}
	
	// Add the tile
	ATilesetManager::AddTileAtLocation(Transform.GetTranslation(), this);
	LastTileLocation = Transform.GetTranslation();
	bAlreadyConstructed = true;

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

void ATileset::Refresh_Implementation(bool bRefreshAdjacent)
{

}

void ATileset::BeginDestroy()
{
	ATilesetManager::RemoveTile(this);

	Super::BeginDestroy();
}