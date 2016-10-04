// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "TilesetManager.h"
#include "WallTileset.h"

AWallTileset::AWallTileset(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Center = CreateDefaultSubobject<UBoxComponent>(TEXT("Center"));
	Center->SetupAttachment(GetRootComponent());
	Center->InitBoxExtent(FVector(64, 64, 64));
	Center->RelativeLocation = FVector(128, 128, 64);
}

void AWallTileset::OnConstruction(const FTransform& Transform)
{
	auto LastTileIndex = TileIndex;

	Super::OnConstruction(Transform);

	// After we have moved our wall, update the adjacent walls in the previous location
	if (bAlreadyConstructed)
	{
		RefreshAdjacentTiles(LastTileIndex);
	}
}

FWallTileAdjacentTiles AWallTileset::GetAdjacentTiles(FTilesetSectorCoordinates Coords, bool bRefreshOverlaps)
{
	auto Adjacents = FWallTileAdjacentTiles();

	if (!IsGameWorld()) return Adjacents;

	auto UpActor = ATilesetManager::GetTileSectorCoords(Coords + FTilesetSectorCoordinates(0, 0, 0, 0, 1));
	auto DownActor = ATilesetManager::GetTileSectorCoords(Coords + FTilesetSectorCoordinates(0, 0, 0, 0, -1));
	auto LeftActor = ATilesetManager::GetTileSectorCoords(Coords + FTilesetSectorCoordinates(0, 0, 0, 1, 0));
	auto RightActor = ATilesetManager::GetTileSectorCoords(Coords + FTilesetSectorCoordinates(0, 0, 0, -1, 0));

	if (IsValid(UpActor) && UpActor != this)
	{
		Adjacents.bUpOverlap = true;

		if (bRefreshOverlaps)
		{
			UpActor->BP_Refresh(false);
		}
	}

	if (IsValid(DownActor) && DownActor != this)
	{
		Adjacents.bDownOverlap = true;

		if (bRefreshOverlaps)
		{
			DownActor->BP_Refresh(false);
		}
	}

	if (IsValid(LeftActor) && LeftActor != this)
	{
		Adjacents.bLeftOverlap = true;

		if (bRefreshOverlaps)
		{
			LeftActor->BP_Refresh(false);
		}
	}

	if (IsValid(RightActor) && RightActor != this)
	{
		Adjacents.bRightOverlap = true;

		if (bRefreshOverlaps)
		{
			RightActor->BP_Refresh(false);
		}
	}

	return Adjacents;
}

void AWallTileset::RefreshAdjacentTiles(FTilesetSectorCoordinates Coords)
{
	if (!IsGameWorld()) return;

	auto UpActor = ATilesetManager::GetTileSectorCoords(Coords + FTilesetSectorCoordinates(0, 0, 0, 0, 1));
	auto DownActor = ATilesetManager::GetTileSectorCoords(Coords + FTilesetSectorCoordinates(0, 0, 0, 0, -1));
	auto LeftActor = ATilesetManager::GetTileSectorCoords(Coords + FTilesetSectorCoordinates(0, 0, 0, 1, 0));
	auto RightActor = ATilesetManager::GetTileSectorCoords(Coords + FTilesetSectorCoordinates(0, 0, 0, -1, 0));

	if (IsValid(UpActor) && UpActor != this)
	{
		UpActor->BP_Refresh(false);
	}

	if (IsValid(DownActor) && DownActor != this)
	{
		DownActor->BP_Refresh(false);
	}

	if (IsValid(LeftActor) && LeftActor != this)
	{
		LeftActor->BP_Refresh(false);
	}

	if (IsValid(RightActor) && RightActor != this)
	{
		RightActor->BP_Refresh(false);
	}
}

void AWallTileset::CleanupTile()
{
	Super::CleanupTile();

	UE_LOG(LogTemp, Log, TEXT("Cleaning up tile"));

	if (bAlreadyConstructed)
	{
		RefreshAdjacentTiles(TileIndex);
	}

	DestroyWalls();
}