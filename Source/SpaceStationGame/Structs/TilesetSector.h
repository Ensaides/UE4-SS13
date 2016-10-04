#pragma once

#include "TilesetSector.generated.h"

class ATileset;

#define TILESETSECTOR_SECTOR_SIZE 32
#define TILESETSECTOR_TILE_SIZE_XY 256.f
#define TILESETSECTOR_TILE_SIZE_Z 512.f

struct FTilesetSector
{
	// The coordinates of the sector
	int32 CoordX;
	int32 CoordY;
	int32 CoordZ;

	// Pointers to the tiles in the sector
	ATileset* Tiles[TILESETSECTOR_SECTOR_SIZE][TILESETSECTOR_SECTOR_SIZE];
};

USTRUCT(Blueprintable)
struct FTilesetSectorCoordinates
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Sector")
		int32 SectorX;

	UPROPERTY(BlueprintReadWrite, Category = "Sector")
		int32 SectorY;

	UPROPERTY(BlueprintReadWrite, Category = "Sector")
		int32 SectorZ;


	UPROPERTY(BlueprintReadWrite, Category = "Tile Coordinate")
		int32 CoordX;

	UPROPERTY(BlueprintReadWrite, Category = "Tile Coordinate")
		int32 CoordY;

	FTilesetSectorCoordinates operator+(const FTilesetSectorCoordinates& Rhs)
	{
		auto ReturnVar = Rhs;

		ReturnVar.SectorX = SectorX + Rhs.SectorX;
		ReturnVar.SectorY = SectorY + Rhs.SectorY;
		ReturnVar.SectorZ = SectorZ + Rhs.SectorZ;

		ReturnVar.CoordX = CoordX + Rhs.CoordX;
		ReturnVar.CoordY = CoordY + Rhs.CoordY;

		return ReturnVar;
	}

	static FTilesetSectorCoordinates GetSectorCoordsFromWorldLoc(FVector Location)
	{
		auto Coordinates = FTilesetSectorCoordinates();

		Coordinates.SectorX = round((Location.X / TILESETSECTOR_TILE_SIZE_XY) / TILESETSECTOR_SECTOR_SIZE);
		Coordinates.SectorY = round((Location.Y / TILESETSECTOR_TILE_SIZE_XY) / TILESETSECTOR_SECTOR_SIZE);
		Coordinates.SectorZ = round((Location.Z / TILESETSECTOR_TILE_SIZE_Z) / TILESETSECTOR_SECTOR_SIZE);

		Coordinates.CoordX = (int32)round((Location.X / TILESETSECTOR_TILE_SIZE_XY)) % TILESETSECTOR_SECTOR_SIZE;
		Coordinates.CoordY = (int32)round((Location.Y / TILESETSECTOR_TILE_SIZE_XY)) % TILESETSECTOR_SECTOR_SIZE;

		if (Coordinates.CoordX < 0)
			Coordinates.CoordX = TILESETSECTOR_SECTOR_SIZE + Coordinates.CoordX;

		if (Coordinates.CoordY < 0)
			Coordinates.CoordY = TILESETSECTOR_SECTOR_SIZE + Coordinates.CoordY;

		return Coordinates;
	};
	
	// Normalizes the coords
	void Normalize()
	{
		if (CoordX < 0)
		{
			SectorX += (CoordX % TILESETSECTOR_SECTOR_SIZE) + 1;

			CoordX %= TILESETSECTOR_SECTOR_SIZE;
			CoordX = TILESETSECTOR_SECTOR_SIZE + CoordX;
		}

		if (CoordY < 0)
		{
			SectorY += (CoordY % TILESETSECTOR_SECTOR_SIZE) + 1;

			CoordY %= TILESETSECTOR_SECTOR_SIZE;
			CoordY = TILESETSECTOR_SECTOR_SIZE + CoordY;
		}
	};

	static FVector GetWorldLocFromSectorCoords(FTilesetSectorCoordinates Coords)
	{
		// Get the sector location
		auto Location = FVector(Coords.SectorX * TILESETSECTOR_TILE_SIZE_XY * TILESETSECTOR_SECTOR_SIZE,
								Coords.SectorY * TILESETSECTOR_TILE_SIZE_XY * TILESETSECTOR_SECTOR_SIZE,
								Coords.SectorZ * TILESETSECTOR_TILE_SIZE_Z);

		// Then add the coord location
		Location += FVector(Coords.CoordX * TILESETSECTOR_TILE_SIZE_XY,
							Coords.CoordY * TILESETSECTOR_TILE_SIZE_XY,
							0.f);

		return Location;
	};

	FTilesetSectorCoordinates()
	{
		SectorX = 0;
		SectorY = 0;
		SectorZ = 0;

		CoordX = 0;
		CoordY = 0;
	};

	FTilesetSectorCoordinates(int32 inSectorX, int32 inSectorY, int32 inSectorZ, int32 inCoordX, int32 inCoordY)
	{
		SectorX = inSectorX;
		SectorY = inSectorY;
		SectorZ = inSectorZ;

		CoordX = inCoordX;
		CoordY = inCoordY;
	};
};

bool operator==(const FTilesetSectorCoordinates& Lhs, const FTilesetSectorCoordinates& Rhs);

bool operator!=(const FTilesetSectorCoordinates& Lhs, const FTilesetSectorCoordinates& Rhs);