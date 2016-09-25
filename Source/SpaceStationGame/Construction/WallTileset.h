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

	FWallTilesetOverlaps()
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
		
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
		USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
		UBoxComponent* Center;

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Get the wall tiles in adjacent cells, optionally updating them on the new tile positions", DisplayName = "Get Adjacent Tiles"), Category = "Tiles")
		FWallTileAdjacentTiles GetAdjacentTiles(bool bRefreshOverlaps);

	UFUNCTION(BlueprintCallable, Category = "Tiles")
		void RefreshAdjacentTiles(FVector Location);
};
