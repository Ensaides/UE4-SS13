// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "Item.h"
#include "JobObject.h"

UJobObject::UJobObject()
{
	if (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer)
	{
		SetUpJobObject();
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