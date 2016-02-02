// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "SpaceStationGameGameState.h"
#include "Item.h"
#include "JobObject.h"

UJobObject::UJobObject()
{
	//UWorld* const World = GetWorld();

	//auto GameState = Cast<ASpaceStationGameGameState>(World->GetGameState());

	//if (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer)
	//{
	//	SetUpJobObject(GameState->GetJobIndex(this->StaticClass()));
	//}
}

void UJobObject::InitializeJob()
{
	UWorld* const World = GetWorld();

	auto GameState = Cast<ASpaceStationGameGameState>(World->GetGameState());

	if (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer)
	{
		SetUpJobObject(GameState->GetJobIndex(this->StaticClass()));
	}
}

void UJobObject::Add(AItem* Item, uint8 inIndex)
{
	StartingInventory.Items.Add(Item);
	StartingInventory.Index.Add(inIndex);
}

AItem* UJobObject::SpawnInventoryActor(TSubclassOf<AItem> Class)
{
	UWorld* const World = GetWorld();

	FActorSpawnParameters SpawnParams;

	SpawnParams.Owner = Cast<AActor>(GetOuter());

	return World->SpawnActor<AItem>(Class, FVector(0,0,0), FRotator(0,0,0), SpawnParams);
}