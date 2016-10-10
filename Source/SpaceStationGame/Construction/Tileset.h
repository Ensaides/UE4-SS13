// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TilesetSector.h"
#include "Tileset.generated.h"

UCLASS()
class SPACESTATIONGAME_API ATileset : public AActor
{
	GENERATED_UCLASS_BODY()

	virtual void PostInitializeComponents() override;

	virtual void PostLoad() override;

#ifdef WITH_EDITOR
	virtual void PostEditImport() override;

	virtual void PostEditMove(bool bFinished) override;
#endif

	AActor* SpawnActorInEditor(UClass* ActorClass, FTransform Transform);

	virtual void Refresh(bool bRefreshAdjacent, const FTransform& Transform);

	virtual void CleanupTile();

	virtual void Destroyed() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile")
		FTilesetSectorCoordinates TileIndex;
	
	UFUNCTION(BlueprintCallable, Category = "Tile")
		FTilesetSectorCoordinates GetTileIndex() { return TileIndex; };

	UFUNCTION(BlueprintCallable, Category = "World")
		bool IsGameWorld();

	bool bAlreadyConstructed;

	bool bCopyConstructed;
};
