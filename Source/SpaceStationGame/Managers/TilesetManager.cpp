// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "Tileset.h"
#include "TilesetManager.h"

ATilesetManager::ATilesetManager(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

ATileset* ATilesetManager::GetTile(FVector Coordinate)
{
	auto Coords = FTilesetSectorCoordinates::GetCoordinatesFromLoc(Coordinate);

	// Look through the tilesets
	for (auto&& TilesetSector : GetSectors())
	{
		// If we found the sector
		if (TilesetSector.CoordX == Coords.SectorX && TilesetSector.CoordY == Coords.SectorY && TilesetSector.CoordZ == Coords.SectorZ)
		{
			return TilesetSector.Tiles[Coords.CoordX][Coords.CoordY];
		}
	}

	return NULL;
}

void ATilesetManager::AddTile(ATileset* NewTile)
{
	auto Coords = FTilesetSectorCoordinates::GetCoordinatesFromLoc(NewTile->GetActorLocation());

	// Look through the tilesets
	for (auto&& TilesetSector : GetSectors())
	{
		// If we found the sector
		if (TilesetSector.CoordX == Coords.SectorX && TilesetSector.CoordY == Coords.SectorY && TilesetSector.CoordZ == Coords.SectorZ)
		{
			if (!TilesetSector.Tiles[Coords.CoordX][Coords.CoordY])
				TilesetSector.Tiles[Coords.CoordX][Coords.CoordY] = NewTile;

			return;
		}
	}

	// If we didnt find the tile sector, create it
	auto NewSector = FTilesetSector();
	NewSector.CoordX = Coords.SectorX;
	NewSector.CoordY = Coords.SectorY;
	NewSector.CoordZ = Coords.SectorZ;

	// Add it in
	NewSector.Tiles[Coords.CoordX][Coords.CoordY] = NewTile;

	GetSectors().push_back(NewSector);
}

void ATilesetManager::AddTileAtLocation(FVector Location, ATileset* NewTile)
{
	auto Coords = FTilesetSectorCoordinates::GetCoordinatesFromLoc(Location);

	// Look through the tilesets
	for (auto&& TilesetSector : GetSectors())
	{
		// If we found the sector
		if (TilesetSector.CoordX == Coords.SectorX && TilesetSector.CoordY == Coords.SectorY && TilesetSector.CoordZ == Coords.SectorZ)
		{
			if (!TilesetSector.Tiles[Coords.CoordX][Coords.CoordY])
				TilesetSector.Tiles[Coords.CoordX][Coords.CoordY] = NewTile;

			return;
		}
	}

	// If we didnt find the tile sector, create it
	auto NewSector = FTilesetSector();
	NewSector.CoordX = Coords.SectorX;
	NewSector.CoordY = Coords.SectorY;
	NewSector.CoordZ = Coords.SectorZ;

	// Add it in
	NewSector.Tiles[Coords.CoordX][Coords.CoordY] = NewTile;

	GetSectors().push_back(NewSector);
}

void ATilesetManager::RemoveTile(ATileset* RemoveTile)
{
	auto Coords = FTilesetSectorCoordinates::GetCoordinatesFromLoc(RemoveTile->GetActorLocation());

	// Look through the tilesets
	for (auto&& TilesetSector : GetSectors())
	{
		// If we found the sector
		if (TilesetSector.CoordX == Coords.SectorX && TilesetSector.CoordY == Coords.SectorY && TilesetSector.CoordZ == Coords.SectorZ)
		{
			TilesetSector.Tiles[Coords.CoordX][Coords.CoordY] = NULL;

			return;
		}
	}
}

void ATilesetManager::RemoveTileAtLocation(FVector Coordinate)
{
	auto Coords = FTilesetSectorCoordinates::GetCoordinatesFromLoc(Coordinate);

	// Look through the tilesets
	for (auto&& TilesetSector : GetSectors())
	{
		// If we found the sector
		if (TilesetSector.CoordX == Coords.SectorX && TilesetSector.CoordY == Coords.SectorY && TilesetSector.CoordZ == Coords.SectorZ)
		{
			TilesetSector.Tiles[Coords.CoordX][Coords.CoordY] = NULL;

			return;
		}
	}
}

void ATilesetManager::PrintTileInfo()
{
	for (auto&& TilesetSector : GetSectors())
	{
		UE_LOG(LogTemp, Warning, TEXT("Sector X: %d, Y: %d"), TilesetSector.CoordX, TilesetSector.CoordY);
		int32 iRow = 0;
		for (auto&& Rows : TilesetSector.Tiles)
		{
			FString Row = "";
			for (auto Elems : Rows)
			{
				if (IsValid(Elems))
				{
					Row += "1 ";
				}
				else
				{
					Row += "0 ";
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("Row %d: %s"), iRow, *Row);
			iRow++;
		}
	}
}
