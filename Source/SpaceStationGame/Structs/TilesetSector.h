#pragma once

class ATileset;

#define TILESETSECTOR_SECTOR_SIZE 32
#define TILESETSECTOR_TILE_SIZE 256.f

struct FTilesetSector
{
	// The coordinates of the sector
	int32 CoordX;
	int32 CoordY;
	int32 CoordZ;

	// Pointers to the tiles in the sector
	ATileset* Tiles[TILESETSECTOR_SECTOR_SIZE][TILESETSECTOR_SECTOR_SIZE];
};

struct FTilesetSectorCoordinates
{
	int32 SectorX;
	int32 SectorY;
	int32 SectorZ;

	int32 CoordX;
	int32 CoordY;

	static FTilesetSectorCoordinates GetCoordinatesFromLoc(FVector Location)
	{
		auto Coordinates = FTilesetSectorCoordinates();

		Coordinates.SectorX = round((Location.X / TILESETSECTOR_TILE_SIZE) / TILESETSECTOR_SECTOR_SIZE);
		Coordinates.SectorY = round((Location.Y / TILESETSECTOR_TILE_SIZE) / TILESETSECTOR_SECTOR_SIZE);
		Coordinates.SectorZ = round((Location.Z / TILESETSECTOR_TILE_SIZE) / TILESETSECTOR_SECTOR_SIZE);

		Coordinates.CoordX = (int32)round((Location.X / TILESETSECTOR_TILE_SIZE)) % TILESETSECTOR_SECTOR_SIZE;
		Coordinates.CoordY = (int32)round((Location.Y / TILESETSECTOR_TILE_SIZE)) % TILESETSECTOR_SECTOR_SIZE;

		if (Coordinates.CoordX < 0)
			Coordinates.CoordX = TILESETSECTOR_SECTOR_SIZE + Coordinates.CoordX;

		if (Coordinates.CoordY < 0)
			Coordinates.CoordY = TILESETSECTOR_SECTOR_SIZE + Coordinates.CoordY;

		return Coordinates;
	};
};