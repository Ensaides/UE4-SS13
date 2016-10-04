// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "TilesetSector.h"

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