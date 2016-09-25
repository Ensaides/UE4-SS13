// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tileset.generated.h"

UCLASS()
class SPACESTATIONGAME_API ATileset : public AActor
{
	GENERATED_UCLASS_BODY()

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		AActor* SpawnActorInEditor(UClass* ActorClass, FTransform Transform);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Visual")
		void Refresh(bool bRefreshAdjacent);

	virtual void BeginDestroy();

	FVector LastTileLocation;
	bool bAlreadyConstructed;
};
