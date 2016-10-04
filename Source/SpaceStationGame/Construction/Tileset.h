// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TilesetSector.h"
#include "Tileset.generated.h"

UCLASS()
class SPACESTATIONGAME_API ATileset : public AActor
{
	GENERATED_UCLASS_BODY()

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		AActor* SpawnActorInEditor(UClass* ActorClass, FTransform Transform);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Visual")
		void BP_Refresh(bool bRefreshAdjacent);

	virtual void CleanupTile();

	virtual void BeginDestroy() override;

	FTilesetSectorCoordinates TileIndex;
	
	UFUNCTION(BlueprintCallable, Category = "Tile")
		FTilesetSectorCoordinates GetTileIndex() { return TileIndex; };

	UFUNCTION(BlueprintCallable, Category = "World")
		bool IsGameWorld();

	bool bAlreadyConstructed;
};
