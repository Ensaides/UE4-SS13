// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Manager.h"
#include "TilesetSector.h"
#include <list>
#include "TilesetManager.generated.h"

class ATileset;

/**
 * 
 */
UCLASS()
class SPACESTATIONGAME_API ATilesetManager : public AManager
{
	GENERATED_UCLASS_BODY()
	
	// The list of all sectors
	static std::list<FTilesetSector>& GetSectors()
	{
		static std::list<FTilesetSector> Sectors = std::list<FTilesetSector>();

		return Sectors;
	}

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Get a tile from world coordinates.", DisplayName = "Get Tile From World Coordinates"), Category = "Tile")
		static ATileset* GetTileWorldCoords(FVector Location);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Get a tile from tileset sector coordinates.", DisplayName = "Get Tile From Sector Coordinates"), Category = "Tile")
		static ATileset* GetTileSectorCoords(FTilesetSectorCoordinates Coords);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Add a tile to the sectors, returning the new tile's coordinate. Will return true if it was successful.", DisplayName = "Add Tile"), Category = "Tile")
		static bool AddTile(ATileset* NewTile, FTilesetSectorCoordinates& OutCoords);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Add a tile to the sectors, specifying a custom location, returning the new tile's coordinate. Will return true if it was successful.", DisplayName = "Add Tile"), Category = "Tile")
		static bool AddTileAtLocation(ATileset* NewTile, FVector Loc, FTilesetSectorCoordinates& OutCoords);

	/*
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Add a tile to the sectors at location", DisplayName = "Add Tile At Location"), Category = "Tile")
		static void AddTileAtLocation(FVector Location, ATileset* NewTile);
	*/

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Remove a tile from the sectors. It will remove it by the supplied location. It will optionally only remove the tile if it matches the supplied actor. Returns true if it was successful.", DisplayName = "Remove Tile"), Category = "Tile")
		static bool RemoveTile(FTilesetSectorCoordinates Coords, ATileset* RemoveTile = NULL, bool bMatchActor = false);
	
	/*
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Remove all tiles matching the actor.", DisplayName = "Remove Tile"), Category = "Tile")
		static void RemoveAllTilesMatchingActor(ATileset* RemoveTile);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Remove a tile from the sectors by Location", DisplayName = "Remove Tile At Location"), Category = "Tile")
		static void RemoveTileAtLocation(FVector Coordinate);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Remove a tile from the sectors by Location. It will only remove the tile if it matches the pointer arg.", DisplayName = "Remove Matching Tile At Location"), Category = "Tile")
		static void RemoveTileAtLocationMatchActor(FVector Coordinate, ATileset* RemoveTile);
	*/

	UFUNCTION(BlueprintCallable, Category = "Debug")
		static void PrintTileInfo();
};
