// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TilesetSector.h"
#include "Tileset.generated.h"

UCLASS()
class SPACESTATIONGAME_API ATileset : public AActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Tile")
		FString TilesetName;

	virtual void OnConstruction(const FTransform & Transform) override;

	virtual void PostInitializeComponents() override;

	virtual void PostLoad() override;

#ifdef WITH_EDITOR
	virtual void PostEditImport() override;

	virtual void PostEditMove(bool bFinished) override;
#endif

	AActor* SpawnActorInEditor(UClass* ActorClass, FTransform Transform);

	virtual void Refresh(bool bRefreshAdjacent, const FTransform& Transform);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Refresh the tile and update its position in the tileset.", DisplayName = "Refresh"), Category = "Tile")
		void BP_Refresh(bool bRefreshAdjacent, const FTransform& Transform) { Refresh(bRefreshAdjacent, Transform); };

	virtual void CleanupTile();

	virtual void Destroyed() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile")
		FTilesetSectorCoordinates TileIndex;
	
	UFUNCTION(BlueprintCallable, Category = "Tile")
		FTilesetSectorCoordinates GetTileIndex() { return TileIndex; };

	UFUNCTION(BlueprintCallable, Category = "World")
		bool IsGameWorld();

	bool bShouldConstruct;

	bool bAlreadyConstructed;

	bool bCopyConstructed;

	bool bBeingMoved;
};
