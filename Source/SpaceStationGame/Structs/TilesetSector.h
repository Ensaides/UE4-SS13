#pragma once

#include "TilesetSector.generated.h"

class ATileset;

#define TILESETSECTOR_SECTOR_SIZE_X 32
#define TILESETSECTOR_SECTOR_SIZE_Y 32
#define TILESETSECTOR_SECTOR_SIZE_Z 1
#define TILESETSECTOR_TILE_SIZE_X 256.f
#define TILESETSECTOR_TILE_SIZE_Y 256.f
#define TILESETSECTOR_TILE_SIZE_Z 512.f

struct FTilesetSector
{
	// The coordinates of the sector
	int32 CoordX;
	int32 CoordY;
	int32 CoordZ;

	// Pointers to the tiles in the sector
	ATileset* Tiles[TILESETSECTOR_SECTOR_SIZE_X][TILESETSECTOR_SECTOR_SIZE_Y];
};

USTRUCT(Blueprintable)
struct FTilesetSectorCoordinates
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sector")
		int32 SectorX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sector")
		int32 SectorY;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sector")
		int32 SectorZ;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile Coordinate")
		int32 CoordX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile Coordinate")
		int32 CoordY;

	FTilesetSectorCoordinates operator+(const FTilesetSectorCoordinates& Rhs);

	static FTilesetSectorCoordinates GetSectorCoordsFromWorldLoc(FVector Location);
	
	// Normalizes the coords
	void Normalize()
	{
		if (CoordX < 0)
		{
			SectorX += (CoordX / TILESETSECTOR_SECTOR_SIZE_X) - 1;

			CoordX %= TILESETSECTOR_SECTOR_SIZE_X;
			CoordX = TILESETSECTOR_SECTOR_SIZE_X + CoordX;
		}

		if (CoordY < 0)
		{
			SectorY += (CoordY / TILESETSECTOR_SECTOR_SIZE_Y) - 1;

			CoordY %= TILESETSECTOR_SECTOR_SIZE_Y;
			CoordY = TILESETSECTOR_SECTOR_SIZE_Y + CoordY;
		}

		if (CoordX > 31)
		{
			SectorX += (CoordX / TILESETSECTOR_SECTOR_SIZE_X);

			CoordX %= TILESETSECTOR_SECTOR_SIZE_X;
		}

		if (CoordY > 31)
		{
			SectorY += (CoordY / TILESETSECTOR_SECTOR_SIZE_Y);

			CoordY %= TILESETSECTOR_SECTOR_SIZE_Y;
		}
	};

	static FVector GetWorldLocFromSectorCoords(FTilesetSectorCoordinates Coords)
	{
		// Get the sector location
		auto Location = FVector(Coords.SectorX * TILESETSECTOR_TILE_SIZE_X * TILESETSECTOR_SECTOR_SIZE_X,
								Coords.SectorY * TILESETSECTOR_TILE_SIZE_Y * TILESETSECTOR_SECTOR_SIZE_Y,
								Coords.SectorZ * TILESETSECTOR_TILE_SIZE_Z);

		// Then add the coord location
		Location += FVector(Coords.CoordX * TILESETSECTOR_TILE_SIZE_X,
							Coords.CoordY * TILESETSECTOR_TILE_SIZE_Y,
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