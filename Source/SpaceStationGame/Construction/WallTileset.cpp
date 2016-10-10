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

	Walls.Init(NULL, 4);
}

void AWallTileset::PostInitializeComponents()
{
	auto LastTileIndex = TileIndex;

	Super::PostInitializeComponents();

	// After we have moved our wall, update the adjacent walls in the previous location
	if (bAlreadyConstructed)
	{
		RefreshAdjacentTiles(LastTileIndex);
	}
}

void AWallTileset::PostLoad()
{
	auto LastTileIndex = TileIndex;

	Super::PostLoad();

	// After we have moved our wall, update the adjacent walls in the previous location
	if (bAlreadyConstructed && IsGameWorld())
	{
		RefreshAdjacentTiles(LastTileIndex);
	}
}

void AWallTileset::PostEditImport()
{
	Walls.Init(NULL, 4);

	Super::PostEditImport();
}

void AWallTileset::PostEditMove(bool bFinished)
{
	if (bFinished && IsGameWorld())
	{
		auto LastTileIndex = TileIndex;

		Super::PostEditMove(bFinished);

		// After we have moved our wall, update the adjacent walls in the previous location
		if (bAlreadyConstructed)
		{
			RefreshAdjacentTiles(LastTileIndex);
		}
	}
	else
	{
		Super::PostEditMove(bFinished);
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
			UpActor->Refresh(false, UpActor->GetActorTransform());
		}
	}

	if (IsValid(DownActor) && DownActor != this)
	{
		Adjacents.bDownOverlap = true;

		if (bRefreshOverlaps)
		{
			DownActor->Refresh(false, DownActor->GetActorTransform());
		}
	}

	if (IsValid(LeftActor) && LeftActor != this)
	{
		Adjacents.bLeftOverlap = true;

		if (bRefreshOverlaps)
		{
			LeftActor->Refresh(false, LeftActor->GetActorTransform());
		}
	}

	if (IsValid(RightActor) && RightActor != this)
	{
		Adjacents.bRightOverlap = true;

		if (bRefreshOverlaps)
		{
			RightActor->Refresh(false, RightActor->GetActorTransform());
		}
	}

	return Adjacents;
}

void AWallTileset::Refresh(bool bRefreshAdjacent, const FTransform& Transform)
{
	UE_LOG(LogTemp, Warning, TEXT("Refresh "));

	DestroyWalls();

	auto AdjacentTiles = GetAdjacentTiles(GetTileIndex(), bRefreshAdjacent);

	if (!AdjacentTiles.bUpOverlap && !Walls[0])
	{
		auto UpTransform = Transform;
		UpTransform.AddToTranslation(WallUpTransform.GetLocation());
		UpTransform.SetRotation((WallUpTransform.GetRotation().Rotator() + UpTransform.GetRotation().Rotator()).Quaternion());

		auto NewWall = SpawnActorInEditor(WallClass, UpTransform);

		if (NewWall)
			Walls[0] = NewWall;
	}

	if (!AdjacentTiles.bDownOverlap && !Walls[1])
	{
		auto DownTransform = Transform;
		DownTransform.AddToTranslation(WallDownTransform.GetLocation());
		DownTransform.SetRotation((WallDownTransform.GetRotation().Rotator() + DownTransform.GetRotation().Rotator()).Quaternion());

		auto NewWall = SpawnActorInEditor(WallClass, DownTransform);

		if (NewWall)
			Walls[1] = NewWall;
	}

	if (!AdjacentTiles.bLeftOverlap && !Walls[2])
	{
		auto LeftTransform = Transform;
		LeftTransform.AddToTranslation(WallLeftTransform.GetLocation());
		LeftTransform.SetRotation((WallLeftTransform.GetRotation().Rotator() + LeftTransform.GetRotation().Rotator()).Quaternion());

		auto NewWall = SpawnActorInEditor(WallClass, LeftTransform);

		if (NewWall)
			Walls[2] = NewWall;
	}

	if (!AdjacentTiles.bRightOverlap && !Walls[3])
	{
		auto RightTransform = Transform;
		RightTransform.AddToTranslation(WallRightTransform.GetLocation());
		RightTransform.SetRotation((WallRightTransform.GetRotation().Rotator() + RightTransform.GetRotation().Rotator()).Quaternion());

		auto NewWall = SpawnActorInEditor(WallClass, RightTransform);

		if (NewWall)
			Walls[3] = NewWall;
	}

}

void AWallTileset::DestroyWalls()
{
	for (auto& Wall : Walls)
	{
		if (IsValid(Wall))
		{
			Wall->Destroy(false, true);

			Wall = NULL;
		}
	}
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
		UpActor->Refresh(false, UpActor->GetActorTransform());
	}

	if (IsValid(DownActor) && DownActor != this)
	{
		DownActor->Refresh(false, DownActor->GetActorTransform());
	}

	if (IsValid(LeftActor) && LeftActor != this)
	{
		LeftActor->Refresh(false, LeftActor->GetActorTransform());
	}

	if (IsValid(RightActor) && RightActor != this)
	{
		RightActor->Refresh(false, RightActor->GetActorTransform());
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