// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "TilesetSector.h"

FTilesetSectorCoordinates FTilesetSectorCoordinates::operator+(const FTilesetSectorCoordinates& Rhs)
{
	auto ReturnVar = Rhs;

	ReturnVar.SectorX = SectorX + Rhs.SectorX;
	ReturnVar.SectorY = SectorY + Rhs.SectorY;
	ReturnVar.SectorZ = SectorZ + Rhs.SectorZ;

	ReturnVar.CoordX = CoordX + Rhs.CoordX;
	ReturnVar.CoordY = CoordY + Rhs.CoordY;

	return ReturnVar;
}

FTilesetSectorCoordinates FTilesetSectorCoordinates::GetSectorCoordsFromWorldLoc(FVector Location)
{
	auto Coordinates = FTilesetSectorCoordinates();

	Coordinates.SectorX = round((Location.X / TILESETSECTOR_TILE_SIZE_X)) / TILESETSECTOR_SECTOR_SIZE_X;
	Coordinates.SectorY = round((Location.Y / TILESETSECTOR_TILE_SIZE_Y)) / TILESETSECTOR_SECTOR_SIZE_Y;
	Coordinates.SectorZ = round((Location.Z / TILESETSECTOR_TILE_SIZE_Z)) / TILESETSECTOR_SECTOR_SIZE_Z;

	Coordinates.CoordX = (int32)round((Location.X / TILESETSECTOR_TILE_SIZE_X)) % TILESETSECTOR_SECTOR_SIZE_X;
	Coordinates.CoordY = (int32)round((Location.Y / TILESETSECTOR_TILE_SIZE_Y)) % TILESETSECTOR_SECTOR_SIZE_Y;

	if (Coordinates.CoordX < 0)
	{
		Coordinates.CoordX = TILESETSECTOR_SECTOR_SIZE_X + Coordinates.CoordX;
		Coordinates.SectorX--;
	}

	if (Coordinates.CoordY < 0)
	{
		Coordinates.CoordY = TILESETSECTOR_SECTOR_SIZE_Y + Coordinates.CoordY;
		Coordinates.SectorY--;
	}

	return Coordinates;
};

bool operator==(const FTilesetSectorCoordinates& Lhs, const FTilesetSectorCoordinates& Rhs)
{
	return Lhs.SectorX == Rhs.SectorX &&
		Lhs.SectorY == Rhs.SectorY &&
		Lhs.SectorZ == Rhs.SectorZ &&
		Lhs.CoordX == Rhs.CoordX &&
		Lhs.CoordY == Rhs.CoordY;
}

bool operator!=(const FTilesetSectorCoordinates& Lhs, const FTilesetSectorCoordinates& Rhs)
{
	return !(Lhs == Rhs);
}