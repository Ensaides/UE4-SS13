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
	FVector PreviousLastTileLocation = LastTileLocation;

	Super::OnConstruction(Transform);

	// After we have moved our wall, update the adjacent walls in the previous location
	if (bAlreadyConstructed)
	{
		RefreshAdjacentTiles(PreviousLastTileLocation);
	}
}

FWallTileAdjacentTiles AWallTileset::GetAdjacentTiles(bool bRefreshOverlaps)
{
	auto Adjacents = FWallTilesetOverlaps();

	auto UpActor = ATilesetManager::GetTile(GetActorLocation() + FVector(0, TILESETSECTOR_TILE_SIZE, 0));
	auto DownActor = ATilesetManager::GetTile(GetActorLocation() + FVector(0, -TILESETSECTOR_TILE_SIZE, 0));
	auto LeftActor = ATilesetManager::GetTile(GetActorLocation() + FVector(-TILESETSECTOR_TILE_SIZE, 0, 0));
	auto RightActor = ATilesetManager::GetTile(GetActorLocation() + FVector(TILESETSECTOR_TILE_SIZE, 0, 0));

	if (UpActor && UpActor != this)
	{
		Adjacents.bUpOverlap = true;

		if (bRefreshOverlaps)
			UpActor->Refresh(false);
	}

	if (DownActor && DownActor != this)
	{
		Adjacents.bDownOverlap = true;

		if (bRefreshOverlaps)
			DownActor->Refresh(false);
	}

	if (LeftActor && LeftActor != this)
	{
		Adjacents.bLeftOverlap = true;

		if (bRefreshOverlaps)
			LeftActor->Refresh(false);
	}

	if (RightActor && RightActor != this)
	{
		Adjacents.bRightOverlap = true;

		if (bRefreshOverlaps)
			RightActor->Refresh(false);
	}

	return Adjacents;
}

void AWallTileset::RefreshAdjacentTiles(FVector Location)
{
	auto UpActor = ATilesetManager::GetTile(GetActorLocation() + FVector(0, TILESETSECTOR_TILE_SIZE, 0));
	auto DownActor = ATilesetManager::GetTile(GetActorLocation() + FVector(0, -TILESETSECTOR_TILE_SIZE, 0));
	auto LeftActor = ATilesetManager::GetTile(GetActorLocation() + FVector(-TILESETSECTOR_TILE_SIZE, 0, 0));
	auto RightActor = ATilesetManager::GetTile(GetActorLocation() + FVector(TILESETSECTOR_TILE_SIZE, 0, 0));

	if (UpActor && UpActor != this)
	{
		UpActor->Refresh(false);
	}

	if (DownActor && DownActor != this)
	{
		DownActor->Refresh(false);
	}

	if (LeftActor && LeftActor != this)
	{
		LeftActor->Refresh(false);
	}

	if (RightActor && RightActor != this)
	{
		RightActor->Refresh(false);
	}
}