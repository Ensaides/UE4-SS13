// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "Tileset.h"
#include "TilesetManager.h"

ATilesetManager::ATilesetManager(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

ATileset* ATilesetManager::GetTileWorldCoords(FVector Location)
{
	auto Coords = FTilesetSectorCoordinates::GetSectorCoordsFromWorldLoc(Location);

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

ATileset* ATilesetManager::GetTileSectorCoords(FTilesetSectorCoordinates Coords)
{
	Coords.Normalize();

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

bool ATilesetManager::AddTile(ATileset* NewTile, FTilesetSectorCoordinates& OutCoords)
{
	OutCoords = FTilesetSectorCoordinates::GetSectorCoordsFromWorldLoc(NewTile->GetActorLocation());

	// Look through the tilesets
	for (auto&& TilesetSector : GetSectors())
	{
		// If we found the sector
		if (TilesetSector.CoordX == OutCoords.SectorX && TilesetSector.CoordY == OutCoords.SectorY && TilesetSector.CoordZ == OutCoords.SectorZ)
		{
			if (!TilesetSector.Tiles[OutCoords.CoordX][OutCoords.CoordY])
			{
				TilesetSector.Tiles[OutCoords.CoordX][OutCoords.CoordY] = NewTile;

				return true;
			}
			else
			{
				return false;
			}
		}
	}

	// If we didnt find the tile sector, create it
	auto NewSector = FTilesetSector();
	NewSector.CoordX = OutCoords.SectorX;
	NewSector.CoordY = OutCoords.SectorY;
	NewSector.CoordZ = OutCoords.SectorZ;

	// Add it in
	NewSector.Tiles[OutCoords.CoordX][OutCoords.CoordY] = NewTile;

	GetSectors().push_back(NewSector);

	return true;
}

/*
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
*/

bool ATilesetManager::RemoveTile(FTilesetSectorCoordinates Coords, ATileset* RemoveTile, bool bMatchActor)
{
	// Look through the tilesets
	for (auto&& TilesetSector : GetSectors())
	{
		// If we found the sector
		if (TilesetSector.CoordX == Coords.SectorX && TilesetSector.CoordY == Coords.SectorY && TilesetSector.CoordZ == Coords.SectorZ)
		{
			if (bMatchActor)
			{
				if (TilesetSector.Tiles[Coords.CoordX][Coords.CoordY] == RemoveTile)
				{
					TilesetSector.Tiles[Coords.CoordX][Coords.CoordY] = NULL;

					return true;
				}
			}
			else
			{
				TilesetSector.Tiles[Coords.CoordX][Coords.CoordY] = NULL;

				return true;
			}

			return false;
		}
	}

	return false;
}

/*
void ATilesetManager::RemoveAllTilesMatchingActor(ATileset* RemoveTile)
{
	for (auto&& TilesetSector : GetSectors())
	{
		for (auto&& Row : TilesetSector.Tiles)
		{
			for (auto&& Tile : Row)
			{
				if (Tile == RemoveTile)
					Tile = NULL;
			}
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

void ATilesetManager::RemoveTileAtLocationMatchActor(FVector Coordinate, ATileset* RemoveTile)
{
	auto Coords = FTilesetSectorCoordinates::GetCoordinatesFromLoc(Coordinate);

	// Look through the tilesets
	for (auto&& TilesetSector : GetSectors())
	{
		// If we found the sector
		if (TilesetSector.CoordX == Coords.SectorX && TilesetSector.CoordY == Coords.SectorY && TilesetSector.CoordZ == Coords.SectorZ)
		{
			if (TilesetSector.Tiles[Coords.CoordX][Coords.CoordY] == RemoveTile)
				TilesetSector.Tiles[Coords.CoordX][Coords.CoordY] = NULL;

			return;
		}
	}
}
*/

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
