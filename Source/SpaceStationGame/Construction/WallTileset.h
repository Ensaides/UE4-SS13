// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tileset.h"
#include "WallTileset.generated.h"

class UBoxComponent;

USTRUCT(Blueprintable)
struct FWallTileAdjacentTiles
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
		bool bUpOverlap;

	UPROPERTY(BlueprintReadWrite)
		bool bDownOverlap;

	UPROPERTY(BlueprintReadWrite)
		bool bLeftOverlap;

	UPROPERTY(BlueprintReadWrite)
		bool bRightOverlap;

	FWallTileAdjacentTiles()
	{
		bUpOverlap = false;
		bDownOverlap = false;
		bLeftOverlap = false;
		bRightOverlap = false;
	}
};

UCLASS()
class SPACESTATIONGAME_API AWallTileset : public ATileset
{
	GENERATED_UCLASS_BODY()

	virtual void PostInitializeComponents() override;

	virtual void PostLoad() override;

#ifdef WITH_EDITOR
	virtual void PostEditImport() override;

	virtual void PostEditMove(bool bFinished) override;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
		USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
		UBoxComponent* Center;

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Get the wall tiles in adjacent cells, optionally updating them on the new tile positions", DisplayName = "Get Adjacent Tiles"), Category = "Tiles")
		FWallTileAdjacentTiles GetAdjacentTiles(FTilesetSectorCoordinates Coords, bool bRefreshOverlaps);
	
	UFUNCTION(BlueprintCallable, Category = "Tiles")
		void RefreshAdjacentTiles(FTilesetSectorCoordinates Coords);

	virtual void Refresh(bool bRefreshAdjacent, const FTransform& Transform) override;

	virtual void DestroyWalls();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walls")
		TArray<AActor*> Walls;

	virtual void CleanupTile() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Walls")
		TSubclassOf<AActor> WallClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Walls")
		FTransform WallUpTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Walls")
		FTransform WallDownTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Walls")
		FTransform WallLeftTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Walls")
		FTransform WallRightTransform;
};
