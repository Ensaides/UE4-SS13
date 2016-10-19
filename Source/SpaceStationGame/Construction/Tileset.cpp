// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "TilesetManager.h"
#include "Tileset.h"

ATileset::ATileset(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAlreadyConstructed = false;
	bCopyConstructed = false;

	TilesetName = "Tileset";
}

void ATileset::PostInitializeComponents()
{
	if (IsGameWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("ON POST INITIIALZIE COMPOENTS!"));

		if (bAlreadyConstructed)
		{
			ATilesetManager::RemoveTile(TilesetName, TileIndex, this, true);
		}

		// If we succeeded in adding the tile, bAlreadyConstructed = true
		bAlreadyConstructed = ATilesetManager::AddTile(TilesetName, this, TileIndex);

		if (bAlreadyConstructed)
		{
			Refresh(true, GetActorTransform());
		}
	}

	Super::PostInitializeComponents();
}

void ATileset::PostLoad()
{
	if (IsGameWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("ON POST LOAD!"));

		if (bAlreadyConstructed)
		{
			ATilesetManager::RemoveTile(TilesetName, TileIndex, this, true);
		}

		// If we succeeded in adding the tile, bAlreadyConstructed = true
		bAlreadyConstructed = ATilesetManager::AddTile(TilesetName, this, TileIndex);

		if (bAlreadyConstructed)
		{
			Refresh(true, GetActorTransform());
		}
	}

	Super::PostLoad();
}

void ATileset::PostEditImport()
{
	UE_LOG(LogTemp, Warning, TEXT("ON COPY!"));

	bCopyConstructed = true;

	Super::PostEditImport();
}

void ATileset::PostEditMove(bool bFinished)
{
	if (bFinished && IsGameWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("ON EDIT MOVE!"));

		if (bAlreadyConstructed)
		{
			ATilesetManager::RemoveTile(TilesetName, TileIndex, this, true);
		}

		// If we succeeded in adding the tile, bAlreadyConstructed = true
		bAlreadyConstructed = ATilesetManager::AddTile(TilesetName, this, TileIndex);

		if (bAlreadyConstructed)
		{
			Refresh(true, GetActorTransform());
		}
	}

	Super::PostEditMove(bFinished);
}

AActor* ATileset::SpawnActorInEditor(UClass* ActorClass, FTransform Transform)
{
	if (GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bAllowDuringConstructionScript = true;
		SpawnParams.bNoFail = true;

		return GetWorld()->SpawnActor(ActorClass, &Transform, SpawnParams);
	}

	return NULL;
}

void ATileset::Refresh(bool bRefreshAdjacent, const FTransform& Transform)
{
	
}

bool ATileset::IsGameWorld()
{
	auto World = GetWorld();
	if (!World)
	{
		return false;
	}

	// World type 1 is the game world, type 2 is the editor world
	if (World->WorldType == 1 || World->WorldType == 2)
	{
		return true;
	}

	return false;
}

void ATileset::CleanupTile()
{
	if (bAlreadyConstructed)
	{
		ATilesetManager::RemoveTile(TilesetName, TileIndex);
	}
}

void ATileset::Destroyed()
{
	UE_LOG(LogTemp, Log, TEXT("*** BeginDestroy() entered. ***"));

	CleanupTile();

	Super::Destroyed();
}