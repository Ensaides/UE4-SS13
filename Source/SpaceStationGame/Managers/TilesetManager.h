// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Manager.h"
#include "TilesetSector.h"
#include <list>
#include <map>
#include "TilesetManager.generated.h"

class ATileset;

typedef std::list<FTilesetSector> Tileset;
typedef std::map<FString, Tileset> TilesetMap;

/**
 * 
 */
UCLASS()
class SPACESTATIONGAME_API ATilesetManager : public AManager
{
	GENERATED_UCLASS_BODY()

	static TilesetMap& GetTilesetMap()
	{
		static TilesetMap NewTilesetMap = TilesetMap();

		return NewTilesetMap;
	}

	// The list of all sectors
	static Tileset& GetTileset(FString TilesetName)
	{
		auto& NewTilesetMap = GetTilesetMap();

		auto&& It = NewTilesetMap.find(TilesetName);

		if (It == NewTilesetMap.end())
		{
			NewTilesetMap[TilesetName] = Tileset();
		}

		return NewTilesetMap[TilesetName];
	}

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Get a tile from world coordinates.", DisplayName = "Get Tile From World Coordinates"), Category = "Tile")
		static ATileset* GetTileWorldCoords(FString TilesetName, FVector Location);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Get a tile from tileset sector coordinates.", DisplayName = "Get Tile From Sector Coordinates"), Category = "Tile")
		static ATileset* GetTileSectorCoords(FString TilesetName, FTilesetSectorCoordinates Coords);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Add a tile to the sectors, returning the new tile's coordinate. Will return true if it was successful.", DisplayName = "Add Tile"), Category = "Tile")
		static bool AddTile(FString TilesetName, ATileset* NewTile, FTilesetSectorCoordinates& OutCoords);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Add a tile to the sectors, specifying a custom location, returning the new tile's coordinate. Will return true if it was successful.", DisplayName = "Add Tile"), Category = "Tile")
		static bool AddTileAtLocation(FString TilesetName, ATileset* NewTile, FVector Loc, FTilesetSectorCoordinates& OutCoords);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Remove a tile from the sectors. It will remove it by the supplied location. It will optionally only remove the tile if it matches the supplied actor. Returns true if it was successful.", DisplayName = "Remove Tile"), Category = "Tile")
		static bool RemoveTile(FString TilesetName, FTilesetSectorCoordinates Coords, ATileset* RemoveTile = NULL, bool bMatchActor = false);

	UFUNCTION(BlueprintCallable, Category = "Debug")
		static void PrintTileInfo();
};
