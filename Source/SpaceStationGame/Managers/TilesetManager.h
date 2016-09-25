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

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Get a tile from world coordinates", DisplayName = "Get Tile"), Category = "Tile")
		static ATileset* GetTile(FVector Coordinate);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Add a tile to the sectors", DisplayName = "Add Tile"), Category = "Tile")
		static void AddTile(ATileset* NewTile);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Add a tile to the sectors at location", DisplayName = "Add Tile At Location"), Category = "Tile")
		static void AddTileAtLocation(FVector Location, ATileset* NewTile);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Remove a tile from the sectors", DisplayName = "Remove Tile"), Category = "Tile")
		static void RemoveTile(ATileset* RemoveTile);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Remove a tile from the sectors by Location", DisplayName = "Remove Tile At Location"), Category = "Tile")
		static void RemoveTileAtLocation(FVector Coordinate);

	UFUNCTION(BlueprintCallable, Category = "Debug")
		static void PrintTileInfo();
};
